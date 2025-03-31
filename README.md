Project game : Pokemon puzzle
  
               BÁO CÁO BÀI TẬP LỚP MÔN LẬP TRÌNH NÂNG CAO
Sinh viên:  Nguyễn Quang Linh _ 24020201		Sinh ngày:   18 / 08 / 2006	
Ngành học:  Công nghệ thông tin		Lớp:  2425II_INT2215_3	
Tên game : Pokemon puzzle
Lối chơi, logic của game:   Game được lấy ý tưởng từ trò chơi nối pokemon cổ điển nhưng đã được biến tấu về cách chơi.  Cụ thể như sau : 
+ Với game nguyên bản ban đầu : Người chơi chỉ có thể chọn được 2 hình pokemon giống nhau nếu tồn tại đường đi giữa nó và chỉ tối đa được rẽ trái hay phải 2 lần trên đường đi	
+ Game chỉnh sửa sau đó : Người chơi có thể chọn được 2 hình pokemon giống nhau nếu tồn tại đường đi giữa chúng, bất kể có bao nhiêu đường gấp khúc.
+ Nhược điểm : Game sẽ trở nên dễ dàng hơn vì không cần thỏa mãn điều kiện đường gấp khúc.
+ Ưu điểm : Với việc có thể chọn 2 hình giống nhau mà tồn tại đường đi giữa chúng bất kể bao nhiêu đường gấp khúc, việc tìm thấy và nhận ra chúng trở nên khó hơn do có nhiều hình xen giữa, việc đó yêu cầu bạn tinh mắt hơn trong khi chơi.
+ Logic chính của game : Game sử dụng thuật toán BFS – Tìm kiếm đường đi theo chiều rộng làm thuật toán cốt lõi của game, để kiểm tra xem 2 hình được chọn có thỏa mãn không, hay đưa ra gợi ý về 2 hình ảnh thỏa mãn cho người chơi.
+ Logic phụ : Chủ yếu là xử lý tải lên hình ảnh, âm thanh sao cho phù hợp, xử lý các biến thời giạn để tạo các hoạt ảnh hay sắp xếp sao cho không bị xung đột với logic của máy và của SDL.
+ Ngoài ra, logic để tạo ra bảng chơi sao cho luôn luôn tồn tại cách chơi thắng chính là random một cách ngẫu nhiên. Do bảng chơi có kích thước 16*10 và mỗi hình ảnh lại có 4 hình giống nhau tồn tại trên bảng chơi, kết hợp với cách chơi đơn giản là “tồn tại” đường đi thỏa mãn không yêu cầu số lần gấp khúc, nên từ đó với mỗi bảng random 1 cách ngẫu nhiên sẽ luôn tồn tại cách chơi chiến thắng ( Có thể chứng minh bằng cách quy nạp với bảng nhỏ và cho dần 4 hình ảnh giống nhau vào ).
Đồ họa, âm thanh: 
+ Hình ảnh : Game có tổng cộng 146 hình ảnh khác nhau bao gồm hình ảnh nền, hình ảnh nhân vật pokemon trong bảng, hình ảnh các phím chức năng và hình ảnh phục vụ cho những hoạt họa khác nhau.
+ Âm thanh : Game có tổng cộng 10 âm thanh hiệu ứng và 2 âm thanh nền.
+ Hoạt ảnh : Game cũng có 4 hoạt ảnh chuyển động khác nhau và nhiều hoạt ảnh bị động ở các phím chức năng hay phục vụ trong khi chơi.
-	Hoạt ảnh sau khi nhấn nút “PLAY” ở màn hình mở đầu.
-	Hoạt ảnh máy bay đuổi bắt tại màn hình mở đầu.
-	Hoạt ảnh khi nhấn các nút hay trong khi chơi.
-	Hoạt ảnh khi kết thúc game.
-	Ngoài ra còn nhiều hoạt ảnh khác như thời gian, phím chức năng,..có kết hợp cùng âm thanh.
	Nhìn chung, game có đầy đủ hoạt ảnh, hình ảnh và âm thanh. Đồ họa thiên về chủ đề công nghệ và không gian, kết hợp với các hình ảnh chibi.
Cấu trúc của project game: Game được chia làm 3 phần chính:
+ Màn hình mở đầu : Bao gồm nhập tên người chơi, hướng dẫn, xem bảng xếp hạng và tùy chọn tắt hay bật âm thanh.
-	Yêu cầu người chơi phải xem hướng dẫn, nhập tên và nhấn Enter thì mới có thể chơi, nếu không làm đủ các bước thì khi nhấn chơi hệ thống sẽ thông báo. Ngoải ra, người chơi còn có thể xem thứ hạng của những người chơi khác.
+ Phần chính của game : Bao gồm bảng chơi, cột đếm và hiển thị thời gian, nút dừng lại, reset màn chơi, gợi ý và số lần được phép đoán sai.
-	Ở phần này người chơi bắt đầu chơi game dựa theo hướng dẫn ở màn hình mở đầu, người chơi có tổng cộng 5 sự trợ giúp và 5 lần đoán sai. Ngoài ra, có thể dừng hay bắt đầu lại game nếu muốn. Trò chơi kết thúc, thua khi đoán sai 5 lần hoặc hết thời gian 20 phút, thắng khi trên bảng không còn hình nào.
+ Phần cuối của game : Hoạt ảnh kết thúc game, số điểm, nút chơi lại, xem bảng xếp hạng của mình hay quay lại trang bắt đầu để người khác chơi tiếp.
-	Hoạt ảnh kết thúc : Một hoạt ảnh và âm thanh cho trường hợp thắng, một cho trường hợp thua.
Các chức năng đã cài được cho game:
+ Phần mở đầu : 
-	Tắt hoặc bật âm thanh trong cả quá trình chơi.
-	Phần nhập tên người chơi, xem hướng dẫn.
-	Bảng xếp hạng lưu lại 12 người chơi điểm cao nhất qua các lượt chơi khác nhau, trong đó có lưu lại tên, điểm và số thứ tự.
+ Phần màn hình chơi : 
-	Một bảng chơi hiển thị hình ảnh các nhân vật pokemon với các âm thanh và hoạt ảnh khác nhau khi chọn, chọn đúng và chọn sai.
-	Chức năng dừng game tạm thời (Dừng cả biến thời gian).
-	Chức năng cài lại thành màn chơi mới.
+ Phần kết thúc :
-	Chức năng tính điểm : Dựa theo thời gian người chơi còn lại khi thắng, nếu thua sẽ là 0 điểm.
-	Chức năng chơi lại, tính điểm lại cho tên người chơi hiện tại.
-	Xem bảng xếp hạng.
-	Chức năng quay lại màn hình bắt đầu để tiếp tục cho 1 người chơi khác chơi.
Các công cụ hỗ trợ bài làm :
-	Về phần code : Chatgpt, Copilot trong VSCode, Google.
-	Chỉnh sửa ảnh đơn giản : Canva, remove.bg để xóa nền, textstudio.com để lấy ảnh chữ.
-	Chỉnh sửa âm thanh : mp3cut.net và audio-joiner.com để cắt và ghép file âm thanh.
-	Đổi tên miền : convertio.co
Nguồn tư liệu : Google ( Không lưu rõ từng nguồn )
-	Một số nguồn cơ bản : 
+ pixabay.com và elements.envato.com để lấy âm thanh.
+ Nhạc nền của HeatleyBros qua youtube, google drive .
+ Các hình ảnh chức năng và hoạt ảnh : Không lưu rõ.
+ Hình ảnh pokemon : https://bit.ly/3Y7Ce98
