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
    && rm -rf /var/lib/apt/lists/* \
    && apt-get clean \
    && pip3 install --no-cache-dir psutil

# 作業ディレクトリの作成
WORKDIR /app

# minishellプロジェクトをコピー
COPY . /app/

# minishellのビルド テスト用ディレクトリの作成（適切な権限で）
RUN make re \
    && mkdir -p /app/test_results && chmod 777 /app/test_results \
    && chmod +x /app/fuzzing_tester/fuzzer.py

# デフォルトコマンド
CMD ["python3", "/app/fuzzing_tester/fuzzer.py"]