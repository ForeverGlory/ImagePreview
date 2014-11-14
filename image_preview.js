(function ($) {
    Drupal.behaviors.image_preview = {
        attach: function (context) {
            $(".image-preview-upload [type=file]", context).on("change", function (e) {
                var files = e.target.files || e.dataTransfer.files;
                console.log(files);
                $.each(files, function () {
                    var file = this;
                    //判断文件大小、类型、文件数量
                    //console.log(file.size);// 153371
                    //console.log(file.type);// image/png
                    var $ul = $(".image-preview ul", context);
                    var reader = new FileReader();
                    reader.onload = function (e) {
                        var $li = $("<li>");
                        var $op = $("<a>", {text: '删除'}).on("click", function () {
                            $li.remove();
                        });
                        var $span = $("<span>", {text: file.name});
                        var $img = $("<img>", {src: e.target.result});
                        $li.append($img, $span, $op);
                        $ul.append($li);
                    };
                    reader.readAsDataURL(file);
                });
            });
        }
    };
})(jQuery);