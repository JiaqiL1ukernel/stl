#!/bin/bash

# 获取当前日期
current_date=$(date "+%Y-%m-%d")

# 定义记录文件名
record_file="plain.txt"

# 判断记录文件是否存在，如果不存在则创建并写入初始行数
if [ ! -f "$record_file" ]; then
  touch "$record_file"
  line_count=$(find . -type f -name "*.h" | xargs cat | wc -l)
  echo "$current_date:$line_count:$line_count:0" >> "$record_file"
  exit 0
fi

# 获取当天的记录行数
record=$(grep "^$current_date:" "$record_file")

# 如果当天记录存在，则更新当前行数和变化量
if [ -n "$record" ]; then
  line_count=$(find . -type f -name "*.h" | xargs cat | wc -l)
  initial_count=$(echo "$record" | cut -d":" -f2)
  change=$(($line_count - $initial_count))
  sed -i "s/^$current_date:[0-9]*:[0-9]*:.*/$current_date:$initial_count:$line_count:$change/" "$record_file"
else
  # 当天记录不存在，则新增一条记录
  line_count=$(find . -type f -name "*.h" | xargs cat | wc -l)
  echo "$current_date:$line_count:$line_count:0" >> "$record_file"
fi

exit 0

