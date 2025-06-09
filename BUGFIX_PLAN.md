# Minishell バグ修正プラン

## テスター結果サマリー
- **テスト通過率**: 95/298 (31.9%)
- **主要問題**: 構文エラー処理、セグメンテーションフォルト、Heredoc未実装、Export/Unset機能不完全

---

## 問題分類と優先度

### 🔴 Critical（最重要・即修正必要）

#### 1. セグメンテーションフォルト対策 
✅ **修正完了**
**問題**: ~~多数のコマンドでSEGFAULT発生~~ → **SEGVを完全修正**
**影響範囲**: 全体のシステム安定性
**修正済み項目**:
✅ `merge_adjacent_tokens`の不完全な結合処理修正
✅ `add_argument`のメモリ管理修正
✅ `main.c`のメモリ管理修正（EOF処理含む）
✅ `builtin_exit`のNULLポインタアクセス修正
✅ `parse_exit_number`のNULLチェック追加
✅ `external_commands.c`のNULLチェック強化
✅ **空のクォート文字列のdouble free修正**

**検証結果**:
- ✅ 基本コマンド(`pwd`, `exit`)が正常動作
- ✅ 空行・空白行のセグメンテーションフォルト修正
- ✅ パイプライン・リダイレクション基本動作確認
- ✅ クォート処理の安定性向上
- ✅ 不正入力でのクラッシュ防止
- ✅ **空のクォート文字列(`""`, `''`)の処理完全修正**
- ✅ **構文エラー処理でexit status 2が正しく返される**

**最新テスト結果**:
- ✅ **builtins**: 43/52 (82.7%) ← 大幅改善
- ✅ **pipes**: 3/4 (75%)
- ✅ **redirects**: 63/75 (84%)
- 総合的に大幅な改善を達成

**発見された新しい問題**:
- ✅ **空のクォート引数のスペース処理問題を完全修正**
  - 修正内容: `add_argument`関数で空文字列も有効な引数として処理
  - 修正内容: `remove_quote_tokens`で空のクォートワードを削除しないよう修正
  - 検証結果: `echo '' b` → ` b` (bashと同じ動作)
- ✅ **cdコマンドの引数エラー処理修正**
  - 修正内容: `builtin_cd`で引数が多すぎる場合のエラーメッセージとexit status 1を追加
  - 検証結果: `cd $PWD hi` → "cd: too many arguments" + exit status 1
- ⚠️ **exitコマンドのexit status処理問題**
  - 問題: テスターでexit後のexit codeが取得できない（プロセス終了のため）
- ⚠️ 引数分割の問題（`ls -l` → `ls-l`として解釈）
- ⚠️ 環境変数展開の問題（`export TEST=value`が正しく処理されない）
- ⚠️ コマンド実行順序の問題（複数行が適切に実行されない）

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
1. **SEGFAULT対策**
   - 全ファイルでNULLポインタチェック強化
   - メモリ管理の徹底見直し
   - エラー処理の統一化

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
- 関数の単一責任原則遵守
- エラーハンドリングの一貫性
- コメントの充実
- 変数名の明確化

### デバッグ支援
- [`debug_utils.c`](srcs/utils/debug_utils.c)のデバッグ機能活用
- ステップ実行可能な構造維持
- ログ出力機能の追加検討

### パフォーマンス
- 不要な文字列コピー削減
- メモリ使用量最適化
- 処理速度の改善

---

## 期待される結果

- **テスト通過率**: 95/298 → 250+/298 (83%+)
- **SEGFAULT**: 完全解消
- **構文エラー**: 適切なexit status返却
- **Heredoc**: 完全動作
- **Export/Unset**: bash互換動作

このプランに従って段階的に修正を進めることで、minishellの安定性と機能性を大幅に向上させることができます。