FROM ubuntu:22.04

# 環境変数の設定
ENV DEBIAN_FRONTEND=noninteractive
ENV LANG=C.UTF-8

# 必要なパッケージのインストール
RUN apt-get update && apt-get install -y \
    build-essential \
    clang \
    make \
    libreadline-dev \
    python3 \
    python3-pip \
    bash \
    coreutils \
    util-linux \
    vim \
    git \
    valgrind \
    && rm -rf /var/lib/apt/lists/* \
    && apt-get clean \
    && pip3 install --no-cache-dir psutil

# timeoutコマンドはcoreutilsに含まれているので個別確認
RUN timeout --version || (echo "timeout command not available" && exit 1)

# Valgrindが正しくインストールされているか確認
RUN valgrind --version || (echo "Valgrind installation failed" && exit 1)

# 作業ディレクトリの作成
WORKDIR /app

# テスト用ディレクトリの作成（適切な権限で）
RUN mkdir -p /app/test_results && chmod 777 /app/test_results \
    && mkdir -p /app/fuzzing_tester

# minishellバイナリをコピー（ビルド時）
COPY minishell /app/minishell
RUN chmod +x /app/minishell

# デフォルトコマンド（実行時マウント前提）
CMD ["bash", "-c", "chmod +x /app/fuzzing_tester/fuzzer.py && python3 /app/fuzzing_tester/fuzzer.py"]