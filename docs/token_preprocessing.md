# トークン前処理システム

## 概要

minishellのトークン前処理システムは、パーサーの解析精度を向上させるために実装された3段階の処理パイプラインです。この機能は特に複雑なクオート処理と隣接トークンの結合を適切に処理するために開発されました。

### 実装背景

従来のパーサーでは、`cat <"1""2""3""4""5"`のような複雑なクオート文字列の処理において、隣接する文字列が正しく結合されずに解析エラーが発生していました。この問題を解決するため、トークナイザーとパーサーの中間に前処理システムを導入しました。

## アーキテクチャ

トークン前処理システムは以下の3段階で構成されています：

```
[トークナイザー] → [変数展開] → [クオート除去] → [隣接結合] → [パーサー]
```

### 1. 変数展開フェーズ (`expand_all_variables`)

**目的**: 環境変数やパラメータ展開を実行

**処理対象トークン**:
- `TOKEN_WORD`: 通常のワードトークン
- `TOKEN_D_QUOTED_WORD`: ダブルクオート内のワードトークン

**処理内容**:
- `$VAR`や`${VAR}`形式の変数を対応する値に展開
- ダブルクオート内では変数展開が有効
- シングルクオート内では変数展開は無効（処理対象外）

**実装詳細**:
```c
int expand_all_variables(t_token *tokens)
{
    t_token *current = tokens;
    char *expanded;
    
    while (current) {
        if (current->type == TOKEN_WORD || 
            current->type == TOKEN_D_QUOTED_WORD) {
            expanded = expand_env_vars(current->content,
                current->type == TOKEN_D_QUOTED_WORD);
            if (!expanded)
                return (0);
            free(current->content);
            current->content = expanded;
        }
        current = current->next;
    }
    return (1);
}
```

### 2. クオート除去フェーズ (`remove_quote_tokens`)

**目的**: クオートトークンを削除し、クオート済みワードを通常のワードに変換

**処理対象トークン**:
- `TOKEN_SINGLE_QUOTE`: シングルクオート文字
- `TOKEN_DOUBLE_QUOTE`: ダブルクオート文字  
- `TOKEN_S_QUOTED_WORD`: シングルクオート内のワード
- `TOKEN_D_QUOTED_WORD`: ダブルクオート内のワード

**処理内容**:
- クオート文字トークンをリストから削除
- クオート済みワードトークンを`TOKEN_WORD`に変換
- リンクリストの整合性を維持

**使用例**:
```
入力: [TOKEN_DOUBLE_QUOTE]["hello"][TOKEN_DOUBLE_QUOTE][TOKEN_SINGLE_QUOTE]["world"][TOKEN_SINGLE_QUOTE]
出力: [TOKEN_WORD("hello")][TOKEN_WORD("world")]
```

### 3. 隣接結合フェーズ (`merge_adjacent_non_meta_tokens`)

**目的**: 隣接する非メタトークンを1つのトークンに結合

**処理対象**: `TOKEN_WORD`タイプのトークン

**処理内容**:
- 連続する`TOKEN_WORD`トークンを結合
- メタ文字（パイプ、リダイレクション等）で区切られた境界は結合しない
- メモリ効率を考慮した結合処理

**使用例**:
```
入力: [TOKEN_WORD("hello")][TOKEN_WORD("world")][TOKEN_WORD("123")]
出力: [TOKEN_WORD("helloworld123")]
```

**実装の特徴**:
```c
t_token *merge_adjacent_non_meta_tokens(t_token *tokens)
{
    t_token *current = tokens;
    // 隣接するTOKEN_WORDを順次結合
    while (current) {
        if (current->type == TOKEN_WORD) {
            // ft_strjoinを使用して効率的に結合
            merged_content = ft_strdup(current->content);
            next = current->next;
            while (next && next->type == TOKEN_WORD) {
                temp = ft_strjoin(merged_content, next->content);
                // メモリ解放とリンク更新
            }
        }
    }
}
```

## メイン関数

### `preprocess_tokens`

すべての前処理段階を統合するメイン関数です：

```c
t_token *preprocess_tokens(t_token *tokens)
{
    t_token *processed_tokens;
    
    if (!tokens)
        return (NULL);
    
    // 1. 変数展開
    if (!expand_all_variables(tokens))
        return (NULL);
    
    // 2. クオート除去
    processed_tokens = remove_quote_tokens(tokens);
    if (!processed_tokens)
        return (NULL);
    
    // 3. 隣接結合
    processed_tokens = merge_adjacent_non_meta_tokens(processed_tokens);
    if (!processed_tokens)
        return (NULL);
    
    return (processed_tokens);
}
```

## 使用例

### ケース1: 基本的なクオート処理
```bash
入力: echo "hello"world
トークナイザー出力: [echo][TOKEN_DOUBLE_QUOTE]["hello"][TOKEN_DOUBLE_QUOTE][world]
前処理後: [echo][helloworld]
```

### ケース2: 変数展開を含む複雑なケース
```bash
環境変数: USER=john
入力: echo "$USER"_test
トークナイザー出力: [echo][TOKEN_DOUBLE_QUOTE]["$USER"][TOKEN_DOUBLE_QUOTE][_test]
変数展開後: [echo][TOKEN_DOUBLE_QUOTE]["john"][TOKEN_DOUBLE_QUOTE][_test]
クオート除去後: [echo][john][_test]
隣接結合後: [echo][john_test]
```

### ケース3: 問題解決例
```bash
入力: cat <"1""2""3""4""5"
トークナイザー出力: [cat][<][TOKEN_DOUBLE_QUOTE]["1"][TOKEN_DOUBLE_QUOTE][TOKEN_DOUBLE_QUOTE]["2"][TOKEN_DOUBLE_QUOTE]...
前処理後: [cat][<][12345]
```

## 既存コードへの統合

### パーサーでの使用方法

前処理システムは[`parse_tokens`](../srcs/parser/parser.c:114)関数の開始時に呼び出されます：

```c
t_command *parse_tokens(t_token *tokens)
{
    // トークン前処理を実行
    tokens = preprocess_tokens(tokens);
    if (!tokens)
        return (NULL);
    
    // 通常のパーシング処理を継続
    // ...
}
```

### ヘッダーファイル

関数プロトタイプは[`parser.h`](../srcs/parser/parser.h:34)で定義されています：

```c
/* Token preprocessing functions */
int     expand_all_variables(t_token *tokens);
t_token *remove_quote_tokens(t_token *tokens);  
t_token *merge_adjacent_non_meta_tokens(t_token *tokens);
t_token *preprocess_tokens(t_token *tokens);
```

## エラーハンドリング

### メモリ割り当てエラー
- 各段階でメモリ割り当て失敗時は`NULL`を返却
- 呼び出し側で適切なエラーメッセージを表示

### 変数展開エラー
- `expand_env_vars`関数の失敗時はエラーメッセージを出力
- 処理を中断して`0`を返却

## 制限事項と今後の改善点

### 現在の制限事項

1. **パフォーマンス**: 大量のトークンに対する処理効率の最適化が必要
2. **メモリ使用量**: 文字列結合時の一時的なメモリ使用量が多い
3. **エラー回復**: 部分的な失敗からの回復機能が限定的

### 今後の改善案

1. **最適化**:
   - トークン結合時のメモリ再利用
   - 処理順序の最適化による高速化

2. **機能拡張**:
   - より複雑な変数展開パターンへの対応
   - エラー詳細情報の拡充

3. **保守性向上**:
   - ユニットテストの追加
   - デバッグ情報出力機能の強化

## テスト観点

### 基本機能テスト
- 各段階の独立した動作確認
- エラーケースでの適切な処理

### 統合テスト  
- 複雑なクオートパターンの処理
- 変数展開との組み合わせ

### 境界値テスト
- 空文字列の処理
- 非常に長い文字列の処理
- メモリ制限での動作

## 参考情報

- メイン実装: [`parser_preprocess.c`](../srcs/parser/parser_preprocess.c)
- ヘッダー定義: [`parser.h`](../srcs/parser/parser.h)  
- トークン定義: [`minishell.h`](../minishell.h:40)
- 変数展開実装: 環境変数処理モジュール

---
最終更新: 2025/06/05
作成者: minishell開発チーム
