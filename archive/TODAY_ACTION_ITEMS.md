# Phase 2: 機能完成度向上 (2025/06/15~)

## 🎯 Phase 1 完了報告 ✅

### 達成した成果 (2025/06/14)
- [x] **SEGFAULT完全解消**: 基本的なechoコマンド系で100%成功
- [x] **構文エラー処理修正**: 不正構文で適切にexit status 2を返す
- [x] **トークナイザー堅牢化**: `safe_create_token`関数の実装
- [x] **メモリ管理強化**: 構文エラー時のメモリリーク解消
- [x] **入力処理安全性向上**: NULL/空文字列チェックの強化

---

## 🚀 次期優先タスク (Phase 2)

### 1. Heredoc機能の完全実装

#### 実装ロードマップ:
1. **デリミタ解析機能** (1-2日)
   - 引用符付きデリミタの処理→不要
   - 変数展開の要否判定

2. **一時ファイル管理** (2日)
   - 安全な一時ファイル作成
   - プロセス終了時の自動クリーンアップ

3. **入力処理ループ** (2-3日)
   - 複数行入力の処理
   - Ctrl-C/Ctrl-Dシグナル処理

#### 対象ファイル:
- [`srcs/parser/heredoc.c`](srcs/parser/heredoc.c)
- [`srcs/parser/heredoc_utils.c`](srcs/parser/heredoc_utils.c)
- [`srcs/parser/heredoc_process.c`](srcs/parser/heredoc_process.c)

#### 検証テストケース:
```bash
cat << EOF
line1
line2
EOF

cat << DELIMITER
variable expansion: $USER
DELIMITER
```

### 2. Export機能の高度化 (3-4日)
**未実装機能**: `+=` 演算子、複雑なスペース処理

#### 実装項目:
```c
// += 演算子の実装
int handle_append_operator(char *name, char *value)
{
    t_env *existing = get_env_node(name);
    if (existing)
        append_to_existing_value(existing, value);
    else
        create_new_env_variable(name, value);
}
```

#### 対象ファイル:
- [`srcs/builtin/builtin_export_set.c`](srcs/builtin/builtin_export_set.c)
- [`srcs/builtin/builtin_export_utils.c`](srcs/builtin/builtin_export_utils.c)

### 3. 環境変数展開の強化 (3-4日)
**問題**: 未定義変数の展開時のスペース処理

#### 修正内容:
```c
// 空の環境変数での引数消失問題の修正
char *expand_env_var(char *var_name)
{
    t_env *env = get_env_node(var_name);
    if (!env)
        return NULL;  // 空文字列ではなくNULLを返す
    return ft_strdup(env->value);
}
```

#### 対象ファイル:
- [`srcs/env/env_expand.c`](srcs/env/env_expand.c)
- [`srcs/parser/parser_preprocess.c`](srcs/parser/parser_preprocess.c)

---

## 🎯 Phase 2 成功指標

### 最低限達成目標 (Must-Have)
- [ ] Heredoc (`<<`) 演算子の基本動作
- [ ] Export `+=` 演算子の実装
- [ ] 環境変数展開での引数消失問題解消

### 理想的達成目標 (Nice-to-Have)
- [ ] 複雑なHeredocケース (引用符付きデリミタ)
- [ ] 高度なExport機能 (スペース含む値)
- [ ] テスター成功率70%以上達成

### 測定方法
```bash
# Heredoc機能テスト
echo 'cat << EOF
test content
EOF' | ./minishell

# Export += テスト
echo 'export VAR=hello
export VAR+=world
echo $VAR' | ./minishell

# 成功率測定
cd minishell_tester && ./tester | grep -E "(OK|KO)" | head -30
```

---

## 📅 今後のスケジュール

### Week 1 (6/15-6/21): Phase 2 実装
- **月曜**: Heredoc基本実装開始
- **火曜**: デリミタ解析と一時ファイル管理
- **水曜**: 入力処理ループとシグナル処理
- **木曜**: Export += 演算子実装
- **金曜**: 環境変数展開修正・テスト

### Week 2 (6/22-6/28): 統合テストと修正
- **月曜**: 全機能統合テスト
- **火曜**: テスター結果分析と修正
- **水曜**: Norm準拠とコード品質向上
- **木曜**: パフォーマンス最適化
- **金曜**: Phase 2 完了確認

### Week 3 (6/29-7/5): Phase 3 準備
- **月曜**: Bonus機能実装可否検討
- **火曜**: 高度bash互換性調査
- **水曜**: 実装優先度決定
- **木曜**: Phase 3 実装開始
- **金曜**: 週次レビューと調整

---

## �️ 日次開発フロー (更新版)

### 朝の状況確認 (10分)
```bash
# 前日の変更内容確認
git log --oneline -5

# 現在のビルド状況確認
make re

# 基本動作確認
echo 'echo hello' | ./minishell
echo 'pwd' | ./minishell
```

### 開発中の確認 (各修正後)
```bash
# ビルド確認
make re

# 該当機能のテスト
# (実装中の機能に応じて適宜調整)

# メモリリークチェック
echo '[テストコマンド]' | ./minishell 2>&1 | grep -c "LeakSanitizer"
```

### 1日の終わりの確認 (15分)
```bash
# 最終動作確認
echo 'echo hello' | ./minishell
echo 'echo "hello world"' | ./minishell

# テスター実行 (進捗確認)
cd minishell_tester && ./tester | head -20 && cd ..

# 進捗記録とコミット
git add . && git commit -m "[日付]: [実装内容]"
```

---

**Phase 1 完了**: 2025年6月14日 18:00  
**Phase 2 開始**: 2025年6月15日  
**次回レビュー**: 2025年6月21日  
**担当**: 42東京 minishell開発チーム