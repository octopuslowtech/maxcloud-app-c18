#!/bin/bash

output_path="/home/octopus/driveD/maxcloud-app-dev"

if [ ! -d "$output_path" ]; then
    echo "Đường dẫn không tồn tại: $output_path"
    exit 1
fi

# Bước 1: Chạy lệnh build Gradle
#echo "Đang chạy lệnh build Gradle để tạo file .so..."
#./gradlew assembleDebug
#if [ $? -ne 0 ]; then
#    echo "Lỗi trong quá trình build Gradle!"
#    exit 1
#fi

# Bước 2: Copy các file .so
echo "Copy các file .so..."
source_dir="app/build/intermediates/merged_native_libs/debug/out/lib"
declare -A abi_dirs
abi_dirs["arm64-v8a"]="arm64-v8a"
abi_dirs["armeabi-v7a"]="armeabi-v7a"

for abi in "${!abi_dirs[@]}"; do
    src_file="$source_dir/${abi_dirs[$abi]}/liboctopus.so"
    dest_dir="$output_path/app/src/main/jniLibs/$abi"

    # Kiểm tra file nguồn tồn tại
    if [ ! -f "$src_file" ]; then
        echo "File không tồn tại: $src_file"
        continue
    fi

    # Tạo thư mục đích nếu chưa tồn tại
    mkdir -p "$dest_dir"

    # Copy file .so vào thư mục đích
    cp "$src_file" "$dest_dir/"
    echo "Đã copy $src_file vào $dest_dir"
done

# Bước 3: Copy các file .h vào include
base_dir="app/src/main/cpp/Base"
include_dir="$output_path/app/src/main/cpp/include"

# Tạo thư mục đích nếu chưa tồn tại
mkdir -p "$include_dir"

# Copy tất cả các file .h trực tiếp vào include
find "$base_dir" -type f -name "*.h" -exec cp {} "$include_dir" \;

# Bước 4: Copy AppCore.cpp
app_core_src="app/src/main/cpp/AppCore.cpp"
app_core_dest="$output_path/app/src/main/cpp/AppCore.cpp"

if [ -f "$app_core_src" ]; then
    # Tạo thư mục đích nếu chưa tồn tại
    mkdir -p "$(dirname "$app_core_dest")"

    # Copy AppCore.cpp
    cp "$app_core_src" "$app_core_dest"
    echo "Đã copy $app_core_src vào $app_core_dest"
else
    echo "File AppCore.cpp không tồn tại: $app_core_src"
fi

# Bước 5: Chạy lệnh Git
echo "Chạy lệnh Git trong thư mục output..."
cd "$output_path"

# Lấy ngày giờ hiện tại
commit_message="Update: $(date '+%Y-%m-%d %H:%M:%S')"

git add .
git commit -a -m "$commit_message"
git push
if [ $? -eq 0 ]; then
    echo "Đã đẩy code lên repository Git thành công!"
else
    echo "Lỗi khi đẩy code lên repository Git!"
fi

echo "Hoàn thành!"
