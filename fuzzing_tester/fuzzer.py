#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import subprocess
import random
import string
import json
import time
from datetime import datetime
import signal
import threading
import resource

class MinishellDangerTester:
    def __init__(self):
        self.bash_path = "/bin/bash"
        self.minishell_path = "/app/minishell"
        self.results_dir = "/app/test_results"
        
        # ARG_MAXの取得と設定
        try:
            self.arg_max = os.sysconf('SC_ARG_MAX')
        except (OSError, ValueError):
            # フォールバック値として 128KB を設定
            self.arg_max = 131072
        
        self.max_input_length = self.arg_max
        self.test_timeout = 30  # 30秒タイムアウト（Valgrind負荷軽減）
        self.valgrind_timeout = 45  # Valgrind専用タイムアウト
        self.use_valgrind = True
        
        # 危険コマンドを積極的に生成するためのリスト
        self.dangerous_commands = [
            "cat /dev/urandom",
            "yes",
            ":(){ :|:& };:",  # fork bomb
            "while true; do echo hello; done",
            "dd if=/dev/zero of=/dev/null",
            "find / -name '*'",
            "cat /proc/*/environ",
            "seq 1 1000000",
        ]
        
        # 危険なパターンを積極的に生成
        self.dangerous_patterns = [
            # 深いパイプチェーン
            " | " * 20,
            # 大量リダイレクション
            " > /tmp/test" * 10,
            " >> /tmp/test" * 10,
            " < /dev/null" * 10,
            # 複雑な構文
            "`cat /etc/passwd`",
            # 無限ループ構造
            "while true; do",
            "until false; do",
            "for i in {1..999999}; do",
        ]
        
        # bashで入力可能な文字セット（より危険な文字重視）
        self.printable_chars = string.printable
        self.dangerous_chars = "|&;<>(){}[]\"'`$\\*?!~#"
        self.metacharacters = "|&;<>(){}[]\"'`$\\"
        
        # 統計情報
        self.stats = {
            "total_tests": 0,
            "differences_found": 0,
            "crashes_found": 0,
            "timeouts": 0,
            "memory_errors_found": 0,
            "memory_leaks_found": 0,
            "boundary_tests": 0,
            "boundary_failures": 0,
            "start_time": datetime.now().isoformat(),
            "arg_max_limit": self.arg_max,
            "dangerous_commands_tested": 0,
            "minishell_crashes": 0,
            "bash_crashes": 0
        }

    def setup_environment(self):
        """テスト環境のセットアップ"""
        try:
            os.makedirs(self.results_dir, exist_ok=True)
            os.chmod(self.results_dir, 0o777)
        except PermissionError:
            print(f"警告: 結果ディレクトリの権限設定に失敗しました: {self.results_dir}")
        except Exception as e:
            print(f"警告: ディレクトリ作成エラー: {e}")
        
        # minishellが存在するかチェック
        if not os.path.exists(self.minishell_path):
            print(f"エラー: {self.minishell_path} が見つかりません")
            sys.exit(1)

    def generate_dangerous_input(self):
        """危険度の高いランダム入力文字列を生成"""
        generation_type = random.choice([
            "dangerous_command",
            "dangerous_pattern", 
            "random_dangerous",
            "boundary_test",
            "complex_syntax"
        ])
        
        if generation_type == "dangerous_command":
            # 危険コマンドベースの生成
            base_cmd = random.choice(self.dangerous_commands)
            if random.choice([True, False]):
                # パイプやリダイレクションを追加
                modifier = random.choice([
                    " | head -n 1000",
                    " | tail -n 1000", 
                    " > /tmp/dangerous_output",
                    " 2>&1",
                    " &",
                    " ; echo done"
                ])
                return base_cmd + modifier
            return base_cmd
            
        elif generation_type == "dangerous_pattern":
            # 危険パターンの組み合わせ
            pattern = random.choice(self.dangerous_patterns)
            base = random.choice(["echo test", "cat /dev/null", "true", "false"])
            return base + pattern
            
        elif generation_type == "boundary_test":
            # 境界値テスト（非常に長い入力）
            length = random.randint(self.arg_max - 100, self.arg_max + 100)
            if length <= 0:
                length = 1000
            
            print(f"\n[DEBUG] 境界値テスト生成中")
            print(f"  - 生成長: {length} bytes")
            print(f"  - ARG_MAX: {self.arg_max} bytes")
            print(f"  - 制限超過: {'Yes' if length > self.arg_max else 'No'}")
            
            base_cmd = random.choice(["echo", "cat << EOF\n", "VAR="])
            if base_cmd == "cat << EOF\n":
                content = "A" * (length - 20)
                result = f"cat << EOF\n{content}\nEOF"
                print(f"  - コマンドタイプ: heredoc")
                print(f"  - 最終サイズ: {len(result)} bytes")
                return result
            elif base_cmd == "VAR=":
                content = "B" * (length - 10)
                result = f'VAR="{content}"; echo $VAR'
                print(f"  - コマンドタイプ: 変数代入")
                print(f"  - 最終サイズ: {len(result)} bytes")
                return result
            else:
                content = "C" * (length - 10)
                result = f'echo "{content}"'
                print(f"  - コマンドタイプ: echo")
                print(f"  - 最終サイズ: {len(result)} bytes")
                return result
                
        elif generation_type == "complex_syntax":
            # 複雑な構文の生成
            templates = [
                "$(echo $(echo $(echo test)))",
                "`cat /proc/version`",
                "${HOME}/test/${USER}",
                "echo test | cat | cat | cat | cat",
                "echo test > /tmp/a > /tmp/b > /tmp/c",
                "echo test && echo ok || echo error",
                "( echo test; echo test2 ) | grep test",
                "{ echo test; echo test2; } 2>&1",
                "echo test; echo test2; echo test3",
                "echo $((1+2+3+4+5))"
            ]
            return random.choice(templates)
            
        else:
            # ランダム危険文字重視
            length = random.randint(10, 1000)
            # 危険文字の比率を高める
            char_pool = self.dangerous_chars * 3 + string.ascii_letters + string.digits
            return ''.join(random.choice(char_pool) for _ in range(length))

    def run_shell_command(self, shell_path, input_str, test_number):
        """シェルコマンドを実行して結果を取得（subprocess互換性対応）"""
        try:
            # ARG_MAX制限チェック（デバッグログ）
            estimated_arg_size = sum(len(str(arg)) for arg in [shell_path, "-c", input_str]) + 100
            if estimated_arg_size > self.arg_max:
                print(f"\n[DEBUG] テスト#{test_number}: ARG_MAX制限違反予測")
                print(f"  - 推定引数サイズ: {estimated_arg_size} bytes")
                print(f"  - ARG_MAX制限: {self.arg_max} bytes")
                print(f"  - 入力長: {len(input_str)} bytes")
                print(f"  - 入力プレビュー: {repr(input_str[:50])}...")
            
            # タイムアウト時間の選択
            if shell_path == self.minishell_path and self.use_valgrind:
                timeout_duration = self.valgrind_timeout
                cmd = [
                    "timeout", str(timeout_duration),
                    "valgrind",
                    "--tool=memcheck",
                    "--leak-check=full",
                    "--show-leak-kinds=all",
                    "--track-origins=yes",
                    "--error-exitcode=42",
                    shell_path,
                    "-c",
                    input_str
                ]
            else:
                timeout_duration = self.test_timeout
                cmd = [
                    "timeout", str(timeout_duration),
                    shell_path, "-c", input_str
                ]
            
            # Python 2.7互換性のため、timeoutをsubprocessに渡さない
            start_time = time.time()
            process = subprocess.Popen(
                cmd,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True
            )
            
            stdout, stderr = process.communicate()
            end_time = time.time()
            return_code = process.returncode
            
            # タイムアウト検出
            execution_time = end_time - start_time
            timeout_occurred = execution_time >= timeout_duration or return_code == 124  # timeout command exit code
            
            # Valgrindエラーの検出
            memory_error = False
            memory_leak = False
            if shell_path == self.minishell_path and self.use_valgrind:
                memory_error = return_code == 42
                memory_leak = "definitely lost" in stderr or "possibly lost" in stderr
            
            return {
                "stdout": stdout,
                "stderr": stderr,
                "return_code": return_code,
                "timeout": timeout_occurred,
                "crashed": return_code < 0 or return_code in [139, 134, 136, 137],  # SEGV, ABRT, etc.
                "memory_error": memory_error,
                "memory_leak": memory_leak,
                "valgrind_output": stderr if shell_path == self.minishell_path and self.use_valgrind else "",
                "execution_time": execution_time
            }
            
        except Exception as e:
            return {
                "stdout": "",
                "stderr": f"EXECUTION_ERROR: {str(e)}",
                "return_code": -1,
                "timeout": False,
                "crashed": True,
                "memory_error": False,
                "memory_leak": False,
                "valgrind_output": "",
                "execution_time": 0
            }

    def compare_results(self, bash_result, minishell_result):
        """bashとminishellの結果を比較"""
        differences = []
        
        # 終了ステータスの比較
        if bash_result["return_code"] != minishell_result["return_code"]:
            differences.append(f"終了ステータス: bash={bash_result['return_code']}, minishell={minishell_result['return_code']}")
        
        # 標準出力の比較（空白を正規化）
        bash_stdout = bash_result["stdout"].strip()
        mini_stdout = minishell_result["stdout"].strip()
        if bash_stdout != mini_stdout:
            differences.append(f"標準出力が異なります")
        
        # クラッシュの比較
        if bash_result["crashed"] != minishell_result["crashed"]:
            differences.append(f"クラッシュ状態: bash={bash_result['crashed']}, minishell={minishell_result['crashed']}")
        
        # タイムアウトの比較
        if bash_result["timeout"] != minishell_result["timeout"]:
            differences.append(f"タイムアウト: bash={bash_result['timeout']}, minishell={minishell_result['timeout']}")
        
        # メモリエラーの検出（minishellのみ）
        if minishell_result.get("memory_error", False):
            differences.append(f"Valgrindメモリエラー検出")
        
        if minishell_result.get("memory_leak", False):
            differences.append(f"Valgrindメモリリーク検出")
        
        return differences

    def log_result(self, input_str, bash_result, minishell_result, differences, test_number):
        """テスト結果をログファイルに記録"""
        timestamp = datetime.now().isoformat()
        log_entry = {
            "test_number": test_number,
            "timestamp": timestamp,
            "input": repr(input_str),
            "input_length": len(input_str),
            "bash_result": bash_result,
            "minishell_result": minishell_result,
            "differences": differences
        }
        
        # 結果ファイル名を決定
        if minishell_result.get("memory_error") or minishell_result.get("memory_leak"):
            filename = f"memory_{test_number}.json"
            self.stats["memory_errors_found"] += 1 if minishell_result.get("memory_error") else 0
            self.stats["memory_leaks_found"] += 1 if minishell_result.get("memory_leak") else 0
        elif differences:
            filename = f"difference_{test_number}.json"
        elif bash_result["crashed"] or minishell_result["crashed"]:
            filename = f"crash_{test_number}.json"
        else:
            return  # 正常な場合は詳細ログを保存しない
        
        filepath = os.path.join(self.results_dir, filename)
        try:
            with open(filepath, 'w', encoding='utf-8') as f:
                json.dump(log_entry, f, indent=2, ensure_ascii=False)
        except Exception as e:
            print(f"\n警告: ログ書き込みエラー: {e}")

    def update_stats(self, differences, bash_result, minishell_result, input_str):
        """統計情報を更新"""
        self.stats["total_tests"] += 1
        
        if differences:
            self.stats["differences_found"] += 1
        
        if bash_result["crashed"]:
            self.stats["bash_crashes"] += 1
        
        if minishell_result["crashed"]:
            self.stats["minishell_crashes"] += 1
            
        if bash_result["crashed"] or minishell_result["crashed"]:
            self.stats["crashes_found"] += 1
        
        if bash_result["timeout"] or minishell_result["timeout"]:
            self.stats["timeouts"] += 1
        
        # 危険コマンドテストの統計
        if any(dangerous in input_str for dangerous in self.dangerous_commands):
            self.stats["dangerous_commands_tested"] += 1

    def save_stats(self):
        """統計情報を保存"""
        self.stats["end_time"] = datetime.now().isoformat()
        stats_file = os.path.join(self.results_dir, "stats.json")
        try:
            with open(stats_file, 'w', encoding='utf-8') as f:
                json.dump(self.stats, f, indent=2, ensure_ascii=False)
        except Exception as e:
            print(f"\n警告: 統計保存エラー: {e}")

    def print_progress(self, test_number, input_str, differences, bash_result, minishell_result):
        """進捗状況を表示（テスト番号明示）"""
        status_indicators = []
        
        if differences:
            status_indicators.append("DIFF")
        if bash_result["crashed"]:
            status_indicators.append("BASH_CRASH")
        if minishell_result["crashed"]:
            status_indicators.append("MINI_CRASH")
        if bash_result["timeout"] or minishell_result["timeout"]:
            status_indicators.append("TIMEOUT")
        if minishell_result.get("memory_error"):
            status_indicators.append("MEM_ERR")
        if minishell_result.get("memory_leak"):
            status_indicators.append("MEM_LEAK")
        
        status_str = " | ".join(status_indicators) if status_indicators else "OK"
        
        # 現在時刻も表示してハング検出を容易に
        current_time = datetime.now().strftime("%H:%M:%S")
        print(f"[{current_time}] [テスト #{test_number:04d}] {status_str} | 入力長:{len(input_str):4d} | 実行時間:{minishell_result.get('execution_time', 0):.2f}s")
        
        # 問題があった場合は入力のプレビューを表示
        if status_indicators:
            preview = input_str[:50] + "..." if len(input_str) > 50 else input_str
            print(f"                         入力: {repr(preview)}")

    def run_danger_testing(self, num_tests=1000):
        """危険度テストを実行"""
        print("=" * 60)
        print("Minishell 危険度テスト開始")
        print("=" * 60)
        print(f"テスト回数: {num_tests}")
        print(f"タイムアウト: {self.test_timeout}秒（固定）")
        print(f"ARG_MAX制限: {self.arg_max}")
        print(f"Valgrind: {'有効' if self.use_valgrind else '無効'}")
        print(f"目的: minishellの頑健性テスト（クラッシュ、メモリエラー検出）")
        print("=" * 60)
        
        try:
            for test_number in range(1, num_tests + 1):
                print("テスト{test_number}\n")
                # 危険な入力を生成
                input_str = self.generate_dangerous_input()
                
                # bashで実行
                bash_result = self.run_shell_command(self.bash_path, input_str, test_number)
                
                # minishellで実行
                minishell_result = self.run_shell_command(self.minishell_path, input_str, test_number)
                
                # 結果を比較
                differences = self.compare_results(bash_result, minishell_result)
                
                # 統計を更新
                self.update_stats(differences, bash_result, minishell_result, input_str)
                
                # 進捗表示（毎回）
                self.print_progress(test_number, input_str, differences, bash_result, minishell_result)
                
                # 問題があれば詳細をログ
                if (differences or bash_result["crashed"] or minishell_result["crashed"] or 
                    minishell_result.get("memory_error") or minishell_result.get("memory_leak")):
                    self.log_result(input_str, bash_result, minishell_result, differences, test_number)
                
        except KeyboardInterrupt:
            print(f"\n\nテストが中断されました (Ctrl+C)")
        
        finally:
            print("\n" + "=" * 60)
            print("危険度テスト完了!")
            print("=" * 60)
            print(f"実行されたテスト: {self.stats['total_tests']}")
            print(f"発見された差異: {self.stats['differences_found']}")
            print(f"総クラッシュ数: {self.stats['crashes_found']}")
            print(f"  - bashクラッシュ: {self.stats['bash_crashes']}")
            print(f"  - minishellクラッシュ: {self.stats['minishell_crashes']}")
            print(f"タイムアウト: {self.stats['timeouts']}")
            print(f"危険コマンドテスト: {self.stats['dangerous_commands_tested']}")
            if self.use_valgrind:
                print(f"メモリエラー: {self.stats['memory_errors_found']}")
                print(f"メモリリーク: {self.stats['memory_leaks_found']}")
            
            self.save_stats()
            print(f"\n結果は {self.results_dir} に保存されました")
            print("- stats.json: 全体統計情報")
            print("- difference_*.json, crash_*.json: 個別の問題詳細")
            if self.use_valgrind:
                print("- memory_*.json: メモリ問題の詳細")

def main():
    """メイン関数"""
    tester = MinishellDangerTester()
    tester.setup_environment()
    
    # コマンドライン引数からテスト回数を取得
    num_tests = 1000
    if len(sys.argv) > 1:
        try:
            num_tests = int(sys.argv[1])
        except ValueError:
            print("警告: 無効なテスト回数。デフォルトの1000回を使用します")
    
    tester.run_danger_testing(num_tests)

if __name__ == "__main__":
    main()