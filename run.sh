#!/bin/bash

# 检查输入参数
if [ $# -ne 2 ]; then
  echo "错误：请输入两个参数，输入的aig文件和输出的txt文件路径。"
  exit 1
fi

# 提取输入参数
input_file="$1"
output_file="$2"

# 切换到bin文件夹
cd bin || exit 2

# 运行imap
./imap

# 运行read_aiger
./read_aiger -f "$input_file"

# 运行ga
./ga -O "$output_file"
