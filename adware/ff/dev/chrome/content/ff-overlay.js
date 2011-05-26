bigbitches.onFirefoxLoad = function(event) {
  document.getElementById("contentAreaContextMenu")
          .addEventListener("popupshowing", function (e){ bigbitches.showFirefoxContextMenu(e); }, false);
};

bigbitches.showFirefoxContextMenu = function(event) {
  // show or hide the menuitem based on what the context menu is on
  document.getElementById("context-bigbitches").hidden = gContextMenu.onImage;
};

window.addEventListener("load", bigbitches.onFirefoxLoad, false);
