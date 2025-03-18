import os

# Thư mục mà bạn muốn bắt đầu tìm kiếm (có thể là ổ C: hoặc thư mục fonts)
search_directory = "C:\\Windows\\Fonts\\"  # Hoặc đường dẫn khác nếu muốn

# Duyệt qua các thư mục và file
for root, dirs, files in os.walk(search_directory):
    for file in files:
        if file.endswith(".ttf"):
            print(os.path.join(root, file))