#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import json
import glob
from collections import defaultdict
from datetime import datetime

class ResultAnalyzer:
    def __init__(self, results_dir):
        self.results_dir = results_dir
        
    def load_stats(self):
        """統計情報を読み込む"""
        stats_file = os.path.join(self.results_dir, "stats.json")
        if os.path.exists(stats_file):
            with open(stats_file, 'r', encoding='utf-8') as f:
                return json.load(f)
        return {}
    
    def load_difference_files(self):
        """差異ファイルを読み込む"""
        pattern = os.path.join(self.results_dir, "difference_*.json")
        files = glob.glob(pattern)
        differences = []
        
        for file_path in files:
            try:
                with open(file_path, 'r', encoding='utf-8') as f:
                    data = json.load(f)
                    differences.append(data)
            except Exception as e:
                print(f"警告: {file_path} の読み込みに失敗: {e}")
        
        return differences
    
    def load_crash_files(self):
        """クラッシュファイルを読み込む"""
        pattern = os.path.join(self.results_dir, "crash_*.json")
        files = glob.glob(pattern)
        crashes = []
        
        for file_path in files:
            try:
                with open(file_path, 'r', encoding='utf-8') as f:
                    data = json.load(f)
                    crashes.append(data)
            except Exception as e:
                print(f"警告: {file_path} の読み込みに失敗: {e}")
        
        return crashes
    
    def analyze_differences(self, differences):
        """差異を分析"""
        if not differences:
            return
        
        print("\n=== 差異分析 ===")
        print(f"総差異数: {len(differences)}")
        
        # 差異タイプ別の集計
        diff_types = defaultdict(int)
        exit_status_diffs = []
        output_diffs = []
        
        for diff in differences:
            for diff_msg in diff.get('differences', []):
                if '終了ステータス' in diff_msg:
                    diff_types['exit_status'] += 1
                    exit_status_diffs.append(diff)
                elif '標準出力' in diff_msg:
                    diff_types['output'] += 1
                    output_diffs.append(diff)
                elif 'クラッシュ' in diff_msg:
                    diff_types['crash_state'] += 1
                elif 'タイムアウト' in diff_msg:
                    diff_types['timeout'] += 1
        
        print(f"終了ステータスの違い: {diff_types['exit_status']}")
        print(f"標準出力の違い: {diff_types['output']}")
        print(f"クラッシュ状態の違い: {diff_types['crash_state']}")
        print(f"タイムアウトの違い: {diff_types['timeout']}")
        
        # サンプル表示
        if exit_status_diffs:
            print(f"\n終了ステータス差異の例:")
            for i, diff in enumerate(exit_status_diffs[:3]):
                print(f"  [{i+1}] 入力: {diff['input'][:50]}...")
                print(f"      差異: {diff['differences']}")
        
        if output_diffs:
            print(f"\n標準出力差異の例:")
            for i, diff in enumerate(output_diffs[:3]):
                print(f"  [{i+1}] 入力: {diff['input'][:50]}...")
                bash_out = diff['bash_result']['stdout'][:100]
                mini_out = diff['minishell_result']['stdout'][:100]
                print(f"      bash出力: {repr(bash_out)}...")
                print(f"      mini出力: {repr(mini_out)}...")
    
    def analyze_crashes(self, crashes):
        """クラッシュを分析"""
        if not crashes:
            return
        
        print(f"\n=== クラッシュ分析 ===")
        print(f"総クラッシュ数: {len(crashes)}")
        
        bash_crashes = 0
        mini_crashes = 0
        
        for crash in crashes:
            bash_result = crash['bash_result']
            mini_result = crash['minishell_result']
            
            if bash_result.get('crashed', False):
                bash_crashes += 1
            if mini_result.get('crashed', False):
                mini_crashes += 1
        
        print(f"bashクラッシュ: {bash_crashes}")
        print(f"minishellクラッシュ: {mini_crashes}")
        
        # クラッシュ例を表示
        print(f"\nクラッシュ例:")
        for i, crash in enumerate(crashes[:3]):
            print(f"  [{i+1}] 入力: {crash['input'][:50]}...")
            bash_result = crash['bash_result']
            mini_result = crash['minishell_result']
            print(f"      bash終了コード: {bash_result['return_code']}")
            print(f"      mini終了コード: {mini_result['return_code']}")
    
    def find_interesting_patterns(self, differences, crashes):
        """興味深いパターンを発見"""
        print(f"\n=== 興味深いパターン ===")
        
        # 特定の文字が含まれる入力での問題
        special_chars = ['|', '&', ';', '<', '>', '(', ')', '{', '}', '[', ']', '"', "'", '`', '$', '\\']
        char_issues = defaultdict(list)
        
        all_issues = differences + crashes
        for issue in all_issues:
            input_str = issue['input']
            for char in special_chars:
                if char in input_str:
                    char_issues[char].append(issue)
        
        print("特殊文字別の問題発生数:")
        for char, issues in sorted(char_issues.items(), key=lambda x: len(x[1]), reverse=True)[:10]:
            print(f"  '{char}': {len(issues)}回")
        
        # 長い入力での問題
        long_inputs = [issue for issue in all_issues if len(issue['input']) > 100]
        if long_inputs:
            print(f"\n長い入力(100文字以上)での問題: {len(long_inputs)}件")
        
        # 空の入力や非常に短い入力での問題
        short_inputs = [issue for issue in all_issues if len(issue['input'].strip()) < 3]
        if short_inputs:
            print(f"短い入力(3文字未満)での問題: {len(short_inputs)}件")
    
    def generate_report(self):
        """レポートを生成"""
        print("=== Minishell Fuzzing 結果分析 ===")
        print(f"結果ディレクトリ: {self.results_dir}")
        print(f"分析時刻: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        
        # 統計情報
        stats = self.load_stats()
        if stats:
            print(f"\n=== 基本統計 ===")
            print(f"総テスト数: {stats.get('total_tests', 0)}")
            print(f"差異発見数: {stats.get('differences_found', 0)}")
            print(f"クラッシュ数: {stats.get('crashes_found', 0)}")
            print(f"タイムアウト数: {stats.get('timeouts', 0)}")
            
            # 成功率の計算
            total = stats.get('total_tests', 0)
            if total > 0:
                success_rate = ((total - stats.get('differences_found', 0) - stats.get('crashes_found', 0)) / total) * 100
                print(f"成功率: {success_rate:.2f}%")
        
        # 差異分析
        differences = self.load_difference_files()
        self.analyze_differences(differences)
        
        # クラッシュ分析
        crashes = self.load_crash_files()
        self.analyze_crashes(crashes)
        
        # パターン分析
        self.find_interesting_patterns(differences, crashes)
        
        # 推奨事項
        print(f"\n=== 推奨事項 ===")
        if differences or crashes:
            print("以下の点を確認することを推奨します:")
            if differences:
                print("- 差異ファイルを詳細に確認し、実装の違いを特定")
            if crashes:
                print("- クラッシュの原因を調査し、メモリ管理やエラーハンドリングを改善")
            print("- 特に問題の多い特殊文字の処理を重点的に確認")
        else:
            print("問題は発見されませんでした。minishellの動作はbashと一致しています。")

def main():
    if len(sys.argv) != 2:
        print("使用方法: python3 analyze_results.py <結果ディレクトリ>")
        sys.exit(1)
    
    results_dir = sys.argv[1]
    if not os.path.exists(results_dir):
        print(f"エラー: ディレクトリが存在しません: {results_dir}")
        sys.exit(1)
    
    analyzer = ResultAnalyzer(results_dir)
    analyzer.generate_report()

if __name__ == "__main__":
    main()