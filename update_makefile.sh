#!/usr/bin/env bash
set -e

SRC_DIR="srcs"
TEMPLATE="Makefile.template"
TARGET="Makefile"

# OS検出
UNAME_S=$(uname -s)

# srcs/*.c を取得 (main.c はテンプレート側で扱う想定なので除外)
# また、OS依存のファイルも除外
if [ "$UNAME_S" = "Darwin" ]; then
  # macOS環境では、Linux用のファイルを除外
  files=( $(find "$SRC_DIR" -type f -name '*.c' ! -name 'main.c' ! -name 'history_utils_linux.c' ! -name 'parser_heredoc_quoted_test.c' | sort) )
else
  # Linux環境では、macOS用のファイルを除外
  files=( $(find "$SRC_DIR" -type f -name '*.c' ! -name 'main.c' ! -name 'history_utils_macos.c' ! -name 'parser_heredoc_quoted_test.c' | sort) )
fi
len=${#files[@]}

# 出力用一時ファイル
tmpfile=$(mktemp)

# テンプレートを1行ずつ読み込みつつ置き換え
while IFS= read -r line; do
  if [ "$line" = "#########" ]; then
    # ここに .c ファイル一覧を注入
    for idx in "${!files[@]}"; do
      fname=${files[$idx]#"$SRC_DIR/"}
      if [ "$idx" -eq $((len-1)) ]; then
        printf "\t%s\n" "$fname" >> "$tmpfile"
      else
        printf "\t%s \\\\\n" "$fname" >> "$tmpfile"
      fi
    done
  else
    # 通常行はそのまま出力
    echo "$line" >> "$tmpfile"
  fi
done < "$TEMPLATE"

# 一時ファイルを Makefile に上書き
mv "$tmpfile" "$TARGET"
# echo "Makefile を更新しました: ${TARGET}"
