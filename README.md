# Đồ án: Simple Paint

## Thông tin chung
Chương trình được viết trên:
- Visual Studio 2015
- Windows 7 SP1 32bit

## Các chức năng đã được cài đặt
### 1.) Vẽ
- Đường thẳng:
- Hình chữ nhật:
	+ Vẽ hình vuông: Giữ SHIFT khi đang vẽ hình chữ nhật
- Hình ellipse:
	+ Vẽ hình tròn: Giữ SHIFT khi đang vẽ hình tròn

### 2.) Đổi màu: 
- Lựa chọn trên Menu: Choose -> Color => hình vẽ tiếp theo sẽ được đổi sang màu đấy
- Hoặc có thể Select một hình bất kì và đổi màu, hình sẽ đổi ngay lập tức

### 3.) Save/Save As/ Open/ New
- Save: Nhấn vào biểu tượng trên thanh công cụ (Hoặc nhấn tổ hợp phím Ctrl + S trên bàn phím)
- Save As: Nhấn vào biểu tượng trên thanh công cụ (Chỉ khi lần đầu tiên Save khi còn Untitlted), hoặc nhấn F12 trên bàn phím (Sử dụng được bất kì)
- Open: Nhấn vào biểu tượng trên thanh công cụ (Hoặc nhấn tổ hợp phím Ctrl + O trên bàn phím)
- New: Nhấn vào biểu tượng trên thanh công cụ (Hoặc nhấn tổ hợp phím Ctrl + N trên bàn phím)

- Lưu ý: Định dạng file: .pnimg (Em gọi là pain image :( vì đồ án này chất chứa nỗi đau của em khi chạy deadlines :(((() được lưu dưới dạng text file
- Khi Save/Save As, khi đặt tên mà quên không kèm thêm .pnimg, thì chương trình sẽ tự thêm phần đuôi đó phía sau.
- Mỗi file .pnimg sẽ có 1 đoạn text nhận dạng mở đầu là "This is a Paint project file"

### 4.) Select:
- Select: 
	+ Select được nhiều hình 1 lúc
	+ Điều kiện để 1 hình được select: Khi TOÀN BỘ hình đó nằm trong hình chữ nhật select (Khi 1 hình không nằm/nằm 1 phần trong hình chữ nhật select, hình đấy sẽ bị bỏ qua)
	+ Có 3 kiểu select:
		* Select khi dùng chức năng Select
		* Select ngay lập tức khi vừa vẽ 1 hình cụ thể (do em mô phỏng theo Paint thật)
		* Select ngay sau khi Paste/Duplicate hình

	+ Các chức năng có thể thực hiện được khi select:
		* Đổi màu: Đã đề cập ở mục 2.) 
		* Move hình: Kéo thả chuột sau khi select
		* Delete hình: Nhấn vào biểu tượng trên thanh công cụ (Hoặc nhấn phím Delete trên bàn phím) khi đang select hình
		* Cut hình: Nhấn vào biểu tượng trên thanh công cụ (Hoặc nhấn tổ hợp phím Ctrl + X trên bàn phím) khi đang select hình
		* Copy hình: Nhấn vào biểu tượng trên thanh công cụ (Hoặc nhấn tổ hợp phím Ctrl + C trên bàn phím) khi đang select hình
		* Duplicate hình: Nhấn SHIFT khi đang Select hình => Khi kéo hình sang 1 bên, sẽ cho ra 1 duplicate (Do em mô phỏng theo Paint)
- Lưu ý:
	+ Cut/Copy trên thanh công cụ sẽ không thể sử dụng được khi không nằm trong trạng thái Select
	+ Việc thực hiện việc Đổi màu khi đang trong trạng thái "Select ngay lập tức khi vừa vẽ 1 hình cụ thể", sẽ không thể Undo lại thành màu cũ được, vì em mô phỏng theo Paint thật

### 5.) Undo/Redo
- Undo: Nhấn vào biểu tượng trên thanh công cụ (Hoặc nhấn tổ hợp phím Ctrl + Z trên bàn phím)
- Redo: Nhấn vào biểu tượng trên thanh công cụ (Hoặc nhấn tổ hợp phím Ctrl + Y trên bàn phím)
- Undo và Redo cho tới hiện tại đều "hoạt động tốt" khi phối hợp với TẤT CẢ các chức năng trên ("hoạt động tốt" tức là cho tới bây giờ, em vẫn chưa thấy bug, không có nghĩa là nó không có bug)

## Linh tinh
- Tilte mặc định của chương trình sẽ là "Untitled - Paint" khi ta vừa mở Paint hoặc vừa chọn New
- Khi 1 file được Open/Save/Save As, tilte nằm ở góc trái trên của Paint sẽ đổi tên theo tên của file được mở, với định dạng "Tên - Paint" (do em mô phỏng theo Paint)
- Chương trình khi vẽ sẽ không tạo ra hiện tượng giật lag trên khu vực làm việc (Kể cả vùng toolbars)
- Chương trình khi vẽ sẽ không gây ra hiện tượng leak memory, kể cả đối với các GDI Object
