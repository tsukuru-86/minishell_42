# Minishell バグ修正プラン

## 手順 (重要)
1. テスター実行: `make test1`
2. 下記の中から項目を選び、修正し、`make re`を通す
3. 一度選んだ項目の修正が完全に完了するまで他の項目に移らないこと
4. 「各項目の修正が完全に完了」とはそのテスターの項目がOKになることをいう
5. 各項目の修正が完全に完了し、テスターがOKになったらコミットすること
6. Normエラー(`norminette`実行結果による)を修正し、コミットすること
7. KOがなくなるまで1.からくり返す

## テスター結果サマリー（2025/06/10 実行）
- **総テスト数**: 295
- **通過数**: 246
- **失敗数**: 59

## 問題分類と優先度

### 🔴 Critical（最重要・即修正必要）

#### 1. セグメンテーションフォルト対策 
✅ **修正完了**
**問題**: ~~多数のコマンドでSEGFAULT発生~~ → **SEGVを完全修正**
**影響範囲**: ~~全般~~ → **解消済み**

~~#### 2. 環境変数配列のヌル終端破壊バグ修正（ft_strlcpy第三引数ミス）~~
~~✅ **修正完了**~~
~~**問題**: create_env_string内でft_strlcpyの第三引数指定ミスにより、環境変数配列のヌル終端が破壊されるバグが発生~~
~~**対応**: 第三引数を正しく設定し、ヌル終端破壊を防止~~

6. **Unset機能改善**
   - 識別子検証強化
   - エラーメッセージ統一

7. **特殊ケース対応** 🆕
   - echo -n オプション複数指定
   - 特殊なexit code処理
   - bash特有の展開（$'', $""）

**新たに発見された/継続中の主な問題（2025/06/10 テスター実行より）**:
- `/bin/ls`, `/bin/ls -l`, `/bin/ls -la` の出力順序・整形がbashと異なる（外部コマンド依存、優先度低）
- `echo -n` オプション複数指定時の挙動不一致（例: `echo -nnnnnnnnnnnnnn -nns -n test` → bash: `test`、minishell: `-nnnnnnnnnnnnnn -nns -n test`）
- heredoc (`<<`) 全般でstatus 127や出力不一致（全KO）
- `export +=`/`export += ndacunh` などの値追加・スペース処理の不備（bash: 追記、minishell: 追記されない/空になる）
- `unset` の無効識別子・エラーstatus不一致（bash: 0、minishell: 1 など）
- `exit` の異常値（オーバーフロー等）でstatus不一致（例: `exit 9223372036854775808` → bash: 2、minishell: 0）
- `$EMPTY` など未定義環境変数展開時の空白・引数処理（先頭空白や引数消失など）
- `< Makefile | printf 'You see me?'` など複雑なリダイレクト・パイプで出力不一致
- bash特有の `$'...'`, `$"..."` 展開未対応（`printf $'hello'` → bash: `hello`、minishell: `$hello`）
- その他: 引数分割、環境変数展開、コマンド実行順序の一部不一致

**今後の対応**:
- トークナイザーの引数分割ロジック修正
- 環境変数処理の改善

#### 2. 構文エラー処理の修正
**問題**: パイプ・リダイレクションの不正構文でexit status 2を返すべきところで0を返している
**影響範囲**: シェルの基本動作
**推定原因**: [`main.c`](srcs/main.c:72)でパーサーエラー時のexit status設定が不適切

**修正対象ファイル**:
- `srcs/main.c` - メインループのエラーハンドリング
- `srcs/parser/parser.c` - パーサーのエラー検出と戻り値
- `srcs/parser/parser_tokens.c` - トークン解析時のエラー処理

**修正手順**:
1. [`parser.c`](srcs/parser/parser.c)でのエラー検出ロジック強化
2. 構文エラー検出時にexit status 2を設定
3. [`main.c`](srcs/main.c:67-72)のエラーハンドリング修正
4. [`error_messages.h`](srcs/error/error_messages.h)の統一されたエラーメッセージ使用

#### 3. エラーメッセージ統一化
**問題**: エラーメッセージが重複している、perror()で済むところでprintf()を使用している
**影響範囲**: エラーハンドリング全般、コードの保守性
**推定原因**: エラーハンドリングの方針が統一されていない

**修正対象ファイル**:
- `srcs/builtin/*.c` - ビルトインコマンドのエラー処理
- `srcs/external/*.c` - 外部コマンド実行のエラー処理
- `srcs/pipeline/*.c` - パイプライン処理のエラー処理
- `srcs/redirect/*.c` - リダイレクションのエラー処理
- `srcs/parser/*.c` - パーサーのエラー処理

**修正手順**:
1. **システムコールエラーの統一**
   - `open()`, `read()`, `write()`, `fork()`, `execve()`, `dup()`, `dup2()`, `pipe()` 等のエラーは`perror()`使用
   - `perror()`でシステムエラーメッセージとerrno値を自動出力
   - 一貫したエラーメッセージフォーマット確保

2. **カスタムエラーメッセージの適用範囲**
   - 構文エラー、識別子エラー等はprintf()/ft_printf_fd()使用
   - [`error_messages.h`](srcs/error/error_messages.h)の統一メッセージ活用
   - ユーザー定義エラーのみカスタムメッセージ

3. **重複エラーハンドリングの統合**
   - 同様のエラー処理を関数化
   - while文のみ使用（for文・do-while文禁止制約遵守）
   - Norm制約内での実装（関数25行以内、引数4つ以下）

4. **エラーメッセージの一貫性確保**
   - システムコール: `perror("プログラム名: 操作名")`
   - カスタムエラー: `ft_printf_fd(2, "プログラム名: エラーメッセージ\n")`
   - 適切なexit status設定

**perror()使用対象システムコール**:
- ファイル操作: `open()`, `read()`, `write()`, `close()`
- プロセス操作: `fork()`, `execve()`, `wait()`, `waitpid()`
- リダイレクション: `dup()`, `dup2()`, `pipe()`
- ディレクトリ操作: `chdir()`, `getcwd()`, `opendir()`, `readdir()`
- ファイル情報: `stat()`, `lstat()`, `fstat()`, `access()`

### 🟡 High（高優先度）

#### 2. 空の環境変数展開問題の修正 🆕
**問題**: 未定義環境変数が展開される際に余分なスペースが挿入される
**影響範囲**: 環境変数処理、引数パース
**推定原因**: [`env_expand.c`](srcs/env/env_expand.c)での空文字列展開処理不備
**🔄 部分解決済み**: 空引用符のリダイレクション単独実行は修正完了（Phase 16）

**具体例**:
```bash
Test [echo $EMPYT abc][KO]
# Minishell: [ abc]  (先頭に余分なスペース)
# Bash: [abc]

Test [$EMPTY echo $EMPYT abc][KO]  
# Minishell: []
# Bash: [abc]
```

**修正対象ファイル**:
- `srcs/env/env_expand.c` - 環境変数展開メイン処理
- `srcs/env/env_expand_utils.c` - 展開補助機能
- `srcs/parser/parser_preprocess.c` - トークン前処理
- `srcs/parser/parser_preprocess_utils2.c` - トークン結合処理

**修正手順**:
1. 未定義環境変数の展開時に空文字列ではなくNULLを返す
2. トークン結合時に空のトークンを適切に除去
3. 引数配列作成時に空の引数をスキップ
4. `$EMPTY command args` パターンでの先頭空文字列の処理改善

#### 3. Heredoc機能の完全実装
**問題**: `<<`演算子が動作せず（status 127）
**影響範囲**: リダイレクション機能
**推定原因**: [`heredoc.c`](srcs/parser/heredoc.c)の実装が不完全

**修正対象ファイル**:
- `srcs/parser/heredoc.c` - Heredoc実装
- `srcs/parser/heredoc_utils.c` - Heredocユーティリティ
- `srcs/parser/parser_token_to_cmd.c` - トークンからコマンドへの変換
- `srcs/redirect/*.c` - リダイレクション処理

**修正手順**:
1. [`heredoc.c`](srcs/parser/heredoc.c)のprocess_heredoc_line関数完成
2. デリミタの正確な比較処理実装
3. 一時ファイル作成とクリーンアップ機能
4. パーサーでのHEREDOCトークン処理強化
5. 変数展開の適切な処理

#### 4. Export機能の修正
**問題**: `+=`演算子、スペース処理が不適切
**影響範囲**: 環境変数管理
**推定原因**: [`builtin_export_set.c`](srcs/builtin/builtin_export_set.c)での演算子処理不完全

**修正対象ファイル**:
- `srcs/builtin/builtin_export.c` - Export基本機能
- `srcs/builtin/builtin_export_set.c` - 変数設定処理
- `srcs/builtin/builtin_export_utils.c` - Export補助機能
- `srcs/builtin/identifier_validator.c` - 識別子検証

**修正手順**:
1. `+=`演算子の検出と処理ロジック実装
2. 既存変数への値追加機能
3. スペースを含む値の適切な処理
4. 引用符の処理改善
5. エラーメッセージの統一

### 🟢 Medium（中優先度）

#### 5. Unset機能の改善
**問題**: 無効識別子の処理が不正確
**影響範囲**: 環境変数管理
**推定原因**: [`builtin_unset.c`](srcs/builtin/builtin_unset.c)の識別子検証不完全

**修正対象ファイル**:
- `srcs/builtin/builtin_unset.c` - Unset基本機能
- `srcs/builtin/identifier_validator.c` - 識別子検証強化

**修正手順**:
1. 識別子検証ロジックの厳密化
2. 不正識別子に対するエラーメッセージ改善
3. [`error_messages.h`](srcs/error/error_messages.h:18)のエラーメッセージ使用

#### 6. 特殊ケース・bash互換性の向上 🆕
**問題**: bash特有の機能や特殊ケースの処理不備
**影響範囲**: コマンドラインシェルとしての互換性

**サブ問題**:

**6.1 echo -n オプション複数指定**
```bash
# 問題例
Test [echo -nnnnnnnnnnnnnn -nns -n test][KO]
# Minishell: [-nnnnnnnnnnnnnn -nns -n test\n]
# Bash: [test] (改行なし)
```
- **修正対象**: `srcs/builtin/builtin_echo.c`
- **修正内容**: -n オプション解析の改善、複数指定時の処理

**6.2 特殊なexit code処理**
```bash
# 問題例  
Test [exit 9223372036854775808][KO]
# Minishell: 0
# Bash: 2 (オーバーフローエラー)
```
- **修正対象**: `srcs/builtin/builtin_exit.c`, `srcs/builtin/builtin_exit_utils.c`
- **修正内容**: オーバーフロー検出とエラー処理

**6.3 bash特有の展開**
```bash
# 問題例
Test [printf $'hello'][KO]  
# Minishell: [$hello]
# Bash: [hello] ($'...'の解釈)

Test [printf $"hello"][KO]
# Minishell: [$hello]  
# Bash: [hello] ($"..."の解釈)
```
- **修正対象**: `srcs/env/env_expand.c`
- **修正内容**: $'...' と $"..." の特殊展開対応

**6.4 ファイルソート順序の違い**
```bash
# 問題例: /bin/ls の出力順序
# 解決困難（外部コマンドの実装依存）
# 優先度: 低（テスト環境依存のため）
```

#### 7. リダイレクションとパイプの高度な組み合わせ 🆕  
**問題**: 複雑なリダイレクション・パイプ組み合わせでの動作差異
**影響範囲**: パイプライン処理

**具体例**:
```bash
Test [< Makefile | printf 'You see me?'][KO]
# Minishell: [] (何も出力されない)
# Bash: [You see me?]
```

**修正対象ファイル**:
- `srcs/pipeline/pipeline.c` - パイプライン処理
- `srcs/redirect/redirect.c` - リダイレクション処理
- `srcs/pipeline/pipeline_process_utils.c` - プロセス間連携

**修正手順**:
1. リダイレクション優先度の見直し
2. パイプとリダイレクションの実行順序調整
3. stdin/stdout の適切な処理

---

## 実装制約への対応

### 42学校制約の遵守
- **for文・do-while文禁止**: while文のみ使用
- **グローバル変数**: [`g_signal`](minishell.h:34)以外禁止
- **static変数**: 関数外static禁止、関数内static使用可
- **関数制限**: 指定された関数のみ使用
- **Norm準拠**: 1ファイル5関数以下、関数25行以内、引数4つ以下、ローカル変数5つ以下

### メモリ管理
- **malloc失敗対策**: 全allocateでNULLチェック
- **メモリリーク防止**: 適切なfree()とエラー時のクリーンアップ
- **double free防止**: free後のポインタNULL化

### エラーハンドリング規約
- **システムコールエラー**: `perror()`で統一（errno自動出力）
- **カスタムエラー**: [`error_messages.h`](srcs/error/error_messages.h)とprintf/ft_printf_fd使用
- **エラーメッセージ形式**:
  - システム: `perror("minishell: 操作名")`
  - カスタム: `ft_printf_fd(2, "minishell: エラーメッセージ\n")`
- **重複コード排除**: 同様のエラー処理は関数化（Norm制約内）
- **while文使用**: for文・do-while文禁止制約の遵守

---

## 修正順序と手順

**🚨 重要**: 各セクション（#### レベル）の修正完了後、必ずテスターを実行して進捗を確認する
```bash
cd minishell_tester-nda-cunh && ./tester && cd ..
```

### Phase 1: 安定性確保（1-2週間）
1. ~~**SEGFAULT対策**~~ ✅ **修正完了**
   - ~~全ファイルでNULLポインタチェック強化~~
   - ~~メモリ管理の徹底見直し~~
   - ~~エラー処理の統一化~~

2. **構文エラー処理修正**
   - パーサーでのエラー検出強化
   - 適切なexit status設定

3. **エラーメッセージ統一化**
   - システムコールエラーでperror()使用
   - カスタムエラーメッセージの統一
   - 重複エラーハンドリングの除去

### Phase 2: 機能完成（2-3週間）
3. **Heredoc完全実装**
   - デリミタ処理の実装
   - 一時ファイル管理
   - 変数展開処理

4. **Export機能強化**
   - `+=`演算子実装
   - スペース・引用符処理改善

### Phase 3: 品質向上（1週間）
5. **Unset機能改善**
   - 識別子検証強化
   - エラーメッセージ統一

---

## テスト方針

### 優先テスト項目
1. **基本構文**: パイプ、リダイレクション、引用符
2. **エラー処理**: 不正構文、存在しないコマンド
3. **メモリ安定性**: 長時間実行、大量データ処理
4. **Heredoc**: 各種デリミタ、変数展開
5. **Export/Unset**: 各種識別子、特殊文字
6. **エラーメッセージ**: システムコールエラー、カスタムエラーメッセージの一貫性

### 検証ツール
- **Valgrind**: メモリリーク検出
- **AddressSanitizer**: メモリエラー検出
- **テスターツール**: 継続的な回帰テスト

---

## 注意事項

### コード品質
- Norm(norminette)遵守
- 関数の単一責任原則遵守
- エラーハンドリングの一貫性
- 変数名の明確化

### デバッグ支援
- [`debug_utils.c`](srcs/utils/debug_utils.c)のデバッグ機能活用
- ステップ実行可能な構造維持
- ログ出力機能の追加検討

---

## 期待される結果

- **テスト通過率**: 95/298 → 250+/298 (83%+) 
  - 📊 **現状**: 244/298 (約82%) ※2025年6月現在
- ~~**SEGFAULT**: 完全解消~~ ✅ **修正完了**
- **構文エラー**: 適切なexit status返却
- **Heredoc**: 完全動作
- **Export/Unset**: bash互換動作

このプランに従って段階的に修正を進めることで、minishellの安定性と機能性を大幅に向上させることができます。