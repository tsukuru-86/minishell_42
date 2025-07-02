#!/usr/bin/env python3
"""MinishellFuzzerクラスのメソッド診断スクリプト"""

import sys
import os
sys.path.append('fuzzing_tester')

from fuzzer import MinishellFuzzer

def diagnose_fuzzer_methods():
    """MinishellFuzzerクラスのメソッドを診断"""
    fuzzer = MinishellFuzzer()
    
    print("=== MinishellFuzzerクラスメソッド診断 ===")
    print("\n利用可能なメソッド:")
    methods = [method for method in dir(fuzzer) if not method.startswith('_')]
    for method in sorted(methods):
        print(f"  - {method}")
    
    print(f"\n総メソッド数: {len(methods)}")
    
    print("\n=== 問題のメソッドチェック ===")
    target_methods = ['run_fuzzing', 'run_shell_command', 'run_safe_command']
    
    for method_name in target_methods:
        if hasattr(fuzzer, method_name):
            print(f"✅ {method_name}: 存在")
        else:
            print(f"❌ {method_name}: 存在しない")
    
    print("\n=== run_fuzzingメソッドの実行テスト ===")
    try:
        # 実際に呼び出してみる（引数0で即終了）
        fuzzer.run_fuzzing(0)
        print("✅ run_fuzzingメソッド実行成功")
    except AttributeError as e:
        print(f"❌ AttributeError: {e}")
    except Exception as e:
        print(f"⚠️  その他のエラー: {e}")

if __name__ == "__main__":
    diagnose_fuzzer_methods()