/* C言語の標準的なヘッダー設計パターン例 */

/* 1. システムライブラリ風のアプローチ (推奨) */
//   - 公開API: minishell.h
//   - モジュール内部: builtin/builtin_internal.h
//   - これがあなたの現在のアプローチ ✅

/* 2. Linux Kernel風のアプローチ */
//   - include/minishell/*.h (公開)
//   - src/builtin/builtin_private.h (内部)

/* 3. GNU風のアプローチ */
//   - minishell/builtin.h (モジュール公開)
//   - minishell/internal/builtin.h (内部)

/* あなたの現在の構造: */
//   ✅ minishell.h (メイン公開API)
//   ✅ srcs/builtin/builtin_commands.h (モジュール内部)
//   ✅ srcs/utils/export_utils.h (ユーティリティ内部)
