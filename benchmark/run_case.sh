#!/bin/bash

# 切换到Small文件夹
cd Large || exit 2

# 遍历Small文件夹中的所有文件夹
for dir in */; do
  # 进入文件夹
  cd "$dir" || exit 2

  # 获取文件夹名称作为输出文件名的前缀
  prefix="${dir%}"

  # 寻找文件夹中的AIG文件
  for input_file in *.aig; do
    # 构建输出文件路径
    log_file="/home/wuhaojie/Desktop/EDA_challenge/output${input_file%.*}.log"

   # 切换到脚本所在的目录的上一层
    script_dir="$(dirname "$(dirname "$0")")"
    # 返回Small文件夹
    cd "../" || exit 2
        # 返回bench文件夹
    cd "../" || exit 2
       
    cd "../" || exit 2
    # 切换到bin文件夹
    cd bin || exit 2
    # 运行imap，并将控制台输出重定向到ilog文件中
    ./imap > "$log_file" <<EOF
read_aiger -f "../benchmark/Large/$dir$input_file"
ga
EOF
    # 返回Small文件夹
    cd "/home/wuhaojie/Desktop/EDA_challenge/benchmark/Large" || exit 2
  done
done
