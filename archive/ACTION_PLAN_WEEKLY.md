# Minishell 週次アクションプラン

## 📅 Week 1: 緊急安定化フェーズ (6/14-6/21)

### Day 1-2: SEGFAULT緊急修正 🔴
**目標**: 基本的なechoコマンドを動作させる

#### 修正対象の特定
```bash
# デバッグ情報付きビルド
make re CFLAGS="-Wall -Wextra -Werror -g -fsanitize=address"

# 最小テストケースで問題特定
echo 'echo hello' | ./minishell
echo 'echo "hello world"' | ./minishell
```

#### 修正手順
1. **[`srcs/utils/empty_input_handler.c`](srcs/utils/empty_input_handler.c)の強化**
   ```c
   int handle_empty_input(char *input)
   {
       if (!input || !*input || ft_strlen(input) == 0)
           return (0);
       if (is_only_whitespace(input))
           return (0);
       return (1);
   }
   ```

2. **[`srcs/tokenizer/tokenizer.c`](srcs/tokenizer/tokenizer.c)の引数処理修正**
   - スペース区切り引数の解析強化
   - メモリ確保失敗時の適切な処理
   - NULLポインタチェックの徹底

3. **[`srcs/main_loop.c`](srcs/main_loop.c)の例外処理**
   - 入力検証の強化
   - エラー時のリソース解放

#### 検証方法
```bash
# 段階的テスト
./minishell
> echo hello
> echo "hello world"  
> echo 'hello world'
> exit
```

### Day 3-4: 構文エラー処理修正 🟡

#### 対象ファイル
- [`srcs/parser/parser.c`](srcs/parser/parser.c)
- [`srcs/tokenizer/tokenizer_syntax_check.c`](srcs/tokenizer/tokenizer_syntax_check.c)

#### 実装内容
```c
// parser.c内
int check_syntax_errors(t_token *tokens)
{
    if (has_consecutive_pipes(tokens))
        return (set_exit_status_and_error(2, ERR_UNEXP_TOKEN));
    if (has_incomplete_redirections(tokens))
        return (set_exit_status_and_error(2, ERR_UNEXP_TOKEN));
    if (starts_or_ends_with_pipe(tokens))
        return (set_exit_status_and_error(2, ERR_UNEXP_TOKEN));
    return (0);
}
```

#### テストケース
```bash
echo '|' | ./minishell && echo "Status: $?"  # Expected: Status 2
echo '| echo hi' | ./minishell && echo "Status: $?"  # Expected: Status 2  
echo 'echo hi |' | ./minishell && echo "Status: $?"  # Expected: Status 2
echo '>' | ./minishell && echo "Status: $?"  # Expected: Status 2
```

### Day 5: Norm準拠修正 📋

#### チェック項目
```bash
# 全ファイルのNormチェック
norminette *.h srcs/ | grep -E "(Error|Warning)"

# 主な修正対象
# 1. 関数行数超過 (25行以内)
# 2. 関数引数超過 (4つ以下)  
# 3. ローカル変数超過 (5つ以下)
# 4. ファイル内関数数超過 (5つ以下)
```

#### 修正戦略
- 長い関数の分割
- 引数の構造体化
- ローカル変数のスコープ最適化

### Day 6-7: 週次検証とバグ修正 ✅

#### 検証手順
```bash
# 1. ビルド確認
make re

# 2. 基本機能テスト  
cd minishell_tester && ./tester | head -20

# 3. 手動テスト
./minishell
> echo hello
> echo "hello world"
> pwd
> env | head -5
> exit
```

#### Week 1 成功基準
- [ ] `echo hello` 100%成功
- [ ] `echo "hello world"` 100%成功  
- [ ] パイプ構文エラーでstatus 2
- [ ] Norm準拠エラー0件
- [ ] テスター基本部分50%以上成功

---

## 📅 Week 2: 機能強化フェーズ (6/22-6/28)

### Day 8-10: Heredoc基本実装 🔧

#### Phase 1: デリミタ解析
```c
// heredoc.c
t_heredoc *parse_heredoc_delimiter(char *delimiter)
{
    t_heredoc *heredoc;
    
    heredoc = init_heredoc(delimiter);
    if (!heredoc)
        return (NULL);
    heredoc->delimiter_is_quoted = has_quotes(delimiter);
    heredoc->delimiter = remove_quotes(delimiter);
    return (heredoc);
}
```

#### Phase 2: 入力処理ループ
```c
int read_heredoc_lines(t_heredoc *heredoc)
{
    char *line;
    
    while ((line = readline("> ")))
    {
        if (ft_strcmp(line, heredoc->delimiter) == 0)
        {
            free(line);
            break;
        }
        append_to_heredoc_content(heredoc, line);
        free(line);
    }
    return (0);
}
```

#### テストケース
```bash
cat << EOF
hello
world  
EOF

cat << 'STOP'
no expansion: $USER
STOP
```

### Day 11-12: Export += 演算子実装 ⚡

#### 実装対象
- [`srcs/builtin/builtin_export_set.c`](srcs/builtin/builtin_export_set.c)

#### 実装内容
```c
int handle_export_append(char *name, char *value)
{
    t_env *existing;
    char *new_value;
    
    existing = get_env_node(name);
    if (!existing)
        return (set_env_node(name, value));
    new_value = ft_strjoin(existing->value, value);
    if (!new_value)
        return (-1);
    free(existing->value);
    existing->value = new_value;
    return (0);
}
```

#### テストケース
```bash
export VAR=hello
export VAR+=world
echo $VAR  # Expected: helloworld

export PATH+=":/new/path"
echo $PATH  # Expected: original_path:/new/path
```

### Day 13-14: 環境変数展開修正 🔄

#### 問題の特定と修正
```c
// env_expand.c
char *expand_variable(char *var_name)
{
    t_env *env;
    
    if (!var_name || !*var_name)
        return (NULL);  // 空文字列の代わりにNULLを返す
    env = get_env_node(var_name);
    if (!env)
        return (NULL);  // 未定義変数もNULLを返す
    return (ft_strdup(env->value));
}
```

#### Week 2 成功基準
- [ ] Heredoc基本動作確認
- [ ] Export += 演算子動作
- [ ] 環境変数展開の改善
- [ ] テスター成功率70%以上

---

## 📅 Week 3-4: 品質向上フェーズ (6/29-7/12)

### Week 3: 高度機能完成

#### Day 15-17: Heredoc完全実装
- 変数展開の制御
- シグナル処理の改善
- エラーハンドリング強化

#### Day 18-19: パイプライン処理改善
- 複雑なパイプライン構成への対応
- プロセス間通信の最適化
- エラー伝播の改善

#### Day 20-21: エラーメッセージ統一化
- システムコールエラーはperror()使用
- カスタムエラーメッセージの統一
- エラーstatus の適切な設定

### Week 4: 最終調整と最適化

#### Day 22-24: 全機能統合テスト
- 複雑なテストケース実行
- エッジケース処理確認
- メモリリーク最終確認

#### Day 25-26: パフォーマンス最適化
- 実行速度の向上
- メモリ使用量の最適化
- 大容量データ処理対応

#### Day 27-28: ドキュメント整備
- 機能仕様書更新
- 使用方法説明の改善
- 開発者向けドキュメント完成

---

## 🔧 日次作業フロー

### 朝の準備 (30分)
```bash
# 1. 前日の変更確認
git log --oneline -5

# 2. 現在の状況確認  
make re
cd minishell_tester && ./tester | head -20 && cd ..

# 3. 今日のタスク確認
cat ACTION_PLAN_WEEKLY.md | grep -A 10 "Day $(date +%d)"
```

### 作業中の確認 (各修正後)
```bash
# 1. コンパイル確認
make re

# 2. 基本動作確認
echo 'echo test' | ./minishell

# 3. 該当機能のテスト
# 具体的なテストケース実行
```

### 夕方の整理 (30分)
```bash
# 1. Norm準拠確認
norminette *.h srcs/ | grep -E "(Error|Warning)"

# 2. 進捗記録
echo "$(date): [完了したタスク内容]" >> PROGRESS.log

# 3. 明日の準備
# 次のタスクの下調べと準備
```

## 🎯 週次マイルストーン

### Week 1 Milestone: 基本安定化
**測定基準**:
- [ ] SEGFAULT発生率: 90%→10%以下
- [ ] echo系コマンド成功率: 0%→100%
- [ ] 構文エラー処理: 未実装→完全実装
- [ ] Norm準拠: エラーあり→エラー0件

### Week 2 Milestone: 機能実装
**測定基準**:
- [ ] Heredoc基本動作: 未実装→基本動作
- [ ] Export += 演算子: 未実装→完全動作
- [ ] 環境変数処理: 問題あり→改善済み
- [ ] テスター成功率: 50%→70%

### Week 3-4 Milestone: 品質向上
**測定基準**:
- [ ] 全機能統合: 個別動作→統合動作
- [ ] エラー処理: 部分的→完全実装
- [ ] パフォーマンス: 基本→最適化済み
- [ ] テスター成功率: 70%→90%+

## 📊 進捗トラッキング

### 日次トラッキング
```bash
# PROGRESS.log に記録
echo "$(date '+%Y-%m-%d'): [タスク名] - [状態] - [次のステップ]" >> PROGRESS.log

# 例:
# 2025-06-14: SEGFAULT修正 - tokenizer.c修正完了 - 明日: parser.c確認
# 2025-06-15: 構文エラー処理 - パイプエラー検出実装 - 明日: リダイレクト処理
```

### 週次評価
```bash
# テスター結果の記録
cd minishell_tester && ./tester > ../test_results_$(date +%Y%m%d).txt && cd ..

# 成功率の計算と記録
grep -c "OK" test_results_$(date +%Y%m%d).txt
```

## ⚠️ 緊急時対応プロセス

### 重大バグ発見時
1. **即座の対応停止**: 現在作業中の機能開発を停止
2. **問題の切り分け**: 最小再現ケースの作成
3. **影響範囲調査**: 他の機能への波及確認
4. **優先修正実行**: バグ修正を最優先で実行
5. **回帰テスト**: 修正が他に影響しないか確認

### スケジュール遅延時
1. **現状評価**: 遅延理由と残作業の正確な把握
2. **優先度見直し**: Must-have vs Nice-to-have の再分類
3. **スコープ調整**: 必要に応じた機能削減
4. **リソース再配分**: 集中すべき領域の明確化

---

**作成日**: 2025年6月14日  
**担当**: minishell開発チーム  
**次回更新**: 毎週日曜日