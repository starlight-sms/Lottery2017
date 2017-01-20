for (var item of data.sort(function (a, b) {
    return Math.random() - Math.random();
})) {
    var dom = '<li><div class="userPic"><img src="images/avatars/{{personName}}.jpg" /></div><div class="msgBox"><span class="userName">{{personName}}：</span>{{content}}</div></li>'
        .replace("{{content}}", item.总结)
        .replace(/{{personName}}/g, item.姓名);
    $(dom).appendTo($("#talkList"));
}
var timerId;
var $ul = $("#con ul");
var liFirstHeight = $ul.find("li:first").height();

$ul.css({
    top: "-" + liFirstHeight - 62 + "px"
});

$(".talkBg").hover(function () {
    stopSwapFrame();
});

$(".talkBg").mouseleave(function () {
    $ul.resume();
    keepsSwapFrame();
})

function swapFrame() {
    $ul.animate({
        top: "-" + liFirstHeight - 62 + "px"
    }, 1500, function () {
        $ul.find("li:first").appendTo($ul);
        liFirstHeight = $ul.find("li:first").height();
        $ul.css({
            top: "0px"
        });
    });
}

function keepsSwapFrame() {
    console.log("keepsSwapFrame");
    timerId = setInterval(function scrolllist() {
        swapFrame();
    }, 3300);
}

function stopSwapFrame() {
    console.log("stopSwapFrame");
    $ul.pause();
    clearInterval(timerId);
}

keepsSwapFrame();