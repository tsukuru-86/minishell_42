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

class MinishellFuzzer:
    def __init__(self):
        self.bash_path = "/bin/bash"
        self.minishell_path = "/app/minishell"
        self.results_dir = "/app/test_results"
        self.max_input_length = 1000
        self.test_timeout = 5
        self.dangerous_commands = [
            "rm", "rmdir", "mv", "cp", "dd", "format", "fdisk",
            "mkfs", "mount", "umount", "shutdown", "reboot", "halt",
            "sudo", "su", "passwd", "chown", "chmod", "kill", "killall"
        ]
        
        # bashで入力可能な文字セット
        self.printable_chars = string.printable
        self.special_chars = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"
        self.metacharacters = "|&;<>(){}[]\"'`$\\"
        
        # 統計情報
        self.stats = {
            "total_tests": 0,
            "differences_found": 0,
            "crashes_found": 0,
            "timeouts": 0,
            "start_time": datetime.now().isoformat()
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

    def generate_random_input(self):
        """ランダムな入力文字列を生成"""
        length = random.randint(1, self.max_input_length)
        
        # 異なる文字セットから選択
        char_sets = [
            self.printable_chars,
            string.ascii_letters + string.digits,
            self.special_chars,
            self.metacharacters,
            string.whitespace
        ]
        
        # ランダムに文字セットを選択
        selected_set = random.choice(char_sets)
        
        # 文字列を生成
        input_str = ''.join(random.choice(selected_set) for _ in range(length))
        
        return input_str

    def is_dangerous_input(self, input_str):
        """危険なコマンドが含まれているかチェック"""
        words = input_str.split()
        if not words:
            return False
            
        # 最初の単語（コマンド）をチェック
        command = words[0].strip()
        return command in self.dangerous_commands

    def run_shell_command(self, shell_path, input_str):
        """シェルコマンドを実行して結果を取得"""
        try:
            # プロセスを開始
            process = subprocess.Popen(
                [shell_path, "-c", input_str],
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                timeout=self.test_timeout
            )
            
            stdout, stderr = process.communicate(timeout=self.test_timeout)
            return_code = process.returncode
            
            return {
                "stdout": stdout,
                "stderr": stderr,
                "return_code": return_code,
                "timeout": False,
                "crashed": return_code < 0
            }
            
        except subprocess.TimeoutExpired:
            process.kill()
            return {
                "stdout": "",
                "stderr": "TIMEOUT",
                "return_code": -1,
                "timeout": True,
                "crashed": False
            }
        except Exception as e:
            return {
                "stdout": "",
                "stderr": f"ERROR: {str(e)}",
                "return_code": -1,
                "timeout": False,
                "crashed": True
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
        
        return differences

    def log_result(self, input_str, bash_result, minishell_result, differences):
        """テスト結果をログファイルに記録"""
        timestamp = datetime.now().isoformat()
        log_entry = {
            "timestamp": timestamp,
            "input": repr(input_str),
            "bash_result": bash_result,
            "minishell_result": minishell_result,
            "differences": differences
        }
        
        # 結果ファイル名を決定
        if differences:
            filename = f"difference_{self.stats['differences_found']}.json"
        elif bash_result["crashed"] or minishell_result["crashed"]:
            filename = f"crash_{self.stats['crashes_found']}.json"
        else:
            return  # 正常な場合は詳細ログを保存しない
        
        filepath = os.path.join(self.results_dir, filename)
        try:
            with open(filepath, 'w', encoding='utf-8') as f:
                json.dump(log_entry, f, indent=2, ensure_ascii=False)
        except PermissionError:
            print(f"\n警告: 結果ファイルの書き込みに失敗しました: {filepath}")
        except Exception as e:
            print(f"\n警告: ログ書き込みエラー: {e}")

    def update_stats(self, differences, bash_result, minishell_result):
        """統計情報を更新"""
        self.stats["total_tests"] += 1
        
        if differences:
            self.stats["differences_found"] += 1
        
        if bash_result["crashed"] or minishell_result["crashed"]:
            self.stats["crashes_found"] += 1
        
        if bash_result["timeout"] or minishell_result["timeout"]:
            self.stats["timeouts"] += 1

    def save_stats(self):
        """統計情報を保存"""
        self.stats["end_time"] = datetime.now().isoformat()
        stats_file = os.path.join(self.results_dir, "stats.json")
        try:
            with open(stats_file, 'w', encoding='utf-8') as f:
                json.dump(self.stats, f, indent=2, ensure_ascii=False)
        except PermissionError:
            print(f"\n警告: 統計ファイルの書き込みに失敗しました: {stats_file}")
        except Exception as e:
            print(f"\n警告: 統計保存エラー: {e}")

    def print_progress(self):
        """進捗状況を表示"""
        print(f"\r実行済みテスト: {self.stats['total_tests']}, "
              f"差異: {self.stats['differences_found']}, "
              f"クラッシュ: {self.stats['crashes_found']}, "
              f"タイムアウト: {self.stats['timeouts']}", end="", flush=True)

    def run_fuzzing(self, num_tests=1000):
        """Fuzzingテストを実行"""
        print("Minishell Fuzzing テスト開始")
        print(f"テスト回数: {num_tests}")
        print("=" * 50)
        
        try:
            test_count = 0
            while test_count < num_tests:
                # ランダム入力を生成
                input_str = self.generate_random_input()
                
                # 危険なコマンドをスキップ
                if self.is_dangerous_input(input_str):
                    continue
                
                test_count += 1
                
                # bashで実行
                bash_result = self.run_shell_command(self.bash_path, input_str)
                
                # minishellで実行
                minishell_result = self.run_shell_command(self.minishell_path, input_str)
                
                # 結果を比較
                differences = self.compare_results(bash_result, minishell_result)
                
                # 統計を更新
                self.update_stats(differences, bash_result, minishell_result)
                
                # 興味深い結果をログ
                if differences or bash_result["crashed"] or minishell_result["crashed"]:
                    self.log_result(input_str, bash_result, minishell_result, differences)
                    print(f"\n[{test_count}] 差異またはクラッシュを発見:")
                    print(f"  入力: {repr(input_str[:50])}")
                    if differences:
                        print(f"  差異: {differences}")
                
                # 進捗表示
                if test_count % 10 == 0:
                    self.print_progress()
                
        except KeyboardInterrupt:
            print(f"\n\nテストが中断されました (Ctrl+C)")
        
        finally:
            print(f"\n\nテスト完了!")
            print(f"実行されたテスト: {self.stats['total_tests']}")
            print(f"発見された差異: {self.stats['differences_found']}")
            print(f"クラッシュ: {self.stats['crashes_found']}")
            print(f"タイムアウト: {self.stats['timeouts']}")
            
            self.save_stats()
            print(f"結果は {self.results_dir} に保存されました")

def main():
    """メイン関数"""
    fuzzer = MinishellFuzzer()
    fuzzer.setup_environment()
    
    # コマンドライン引数からテスト回数を取得
    num_tests = 1000
    if len(sys.argv) > 1:
        try:
            num_tests = int(sys.argv[1])
        except ValueError:
            print("警告: 無効なテスト回数。デフォルトの1000回を使用します")
    
    fuzzer.run_fuzzing(num_tests)

if __name__ == "__main__":
    main()