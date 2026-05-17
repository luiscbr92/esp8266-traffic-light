FROM node:20-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
    python3 \
    python3-pip \
    python3-venv \
    gcc \
    g++ \
    git \
    curl \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

RUN python3 -m venv /opt/pio-venv \
    && /opt/pio-venv/bin/pip install --no-cache-dir platformio

ENV PATH="/opt/pio-venv/bin:${PATH}"

RUN npm install -g @anthropic-ai/claude-code

RUN mkdir -p /workspace /home/node/.platformio \
    && chown -R node:node /workspace /home/node/.platformio

USER node
WORKDIR /workspace

ENV PLATFORMIO_CORE_DIR=/home/node/.platformio

CMD ["bash"]