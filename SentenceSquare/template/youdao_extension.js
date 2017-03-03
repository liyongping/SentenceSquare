$(function(){
	var $all_a = $("a.sp.dictvoice.voice-js.log-js");//$("ul.ol");
	$all_a.each(function() {
		var current = $(this);
		if(current.siblings("img.savetolocal").length==0){
			var cparent = current.parent();
			var psiblings = cparent.siblings();
			var zn_p = psiblings[0];
			var link_a = $(psiblings[1]).children('a')[0];
			
			var sentence = {};

			sentence.data_rel = current.attr("data-rel").trim();
			sentence.en_data = cparent.get(0).innerText;
			sentence.zh_data = zn_p.innerText;
			sentence.example_text = $(link_a).text().trim();
			sentence.example_href = $(link_a).attr('href');
			//extra_item = $("<a class='savetolocal'>SAVE</a>");
			extra_item = $("<img class='savetolocal' title='Store Up' alt='Save' style='margin:-5px 6px;' src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABgAAAAYCAYAAADgdz34AAAEfklEQVRIiZWWa2yTVRjHf++lb7uyMdaVbTBggwEu3JxFCRCJyFTwgyzeuCRTQwgJ3xYkMdFoTExMSEg0GD84o5iJEIkxbiECA2vAcRuF0ayAuwDbGN2kg27taLe17/seP6ywbt1Fn+RN3pznf/7/85zznOc5ElObDKiADdASYzFgENABc7LJ0iQ+Fchh/eoStmwuZdbsYqalz0BIEI704e9u4tcaN3UXvEAgIfafBGTAUbDz9U3azjcruhYVuWySRV4m2cnCCgIexHUaYkNE44ZJa1sDPx09wOGjJ4HgVBHJQGH5z59Xvd/jNj4LXRUXhnrEkGmIsTZgGuKXSFAs624S3L5q8P1XVUBhgmNC8nmfntx/4mDvKeEZaBPGOMRjLW6a4ov+bmHzXxP89sMJYN5EIo5PKvdW/n6/WviHuqYkHmu1g0EhBzyC7/ZVAo6x5Nq2LaXlF32H4h0PfROz/HlaiIvnJnR/He0UuYHz8bRdb5WTyLjHoTjfe2VdRV48Q52btWT8DbzVBo90aOmEYN+4kN22fHbZ0tTnd2yuAJyPBeTSpU+VrEib4cqduRRJmuCMLvngWRc844L6xnEhiiSx3ZrPqsX5rqKXV5UAsgyo5StKStP7TNmWVziCNgwIBEe++w9hVg4sWQhNd0bGe4JgjmTmYks2RYqQ120tLQVUFbAtkOzFaXIGkqo+AYrKw0i5M0cEN6xKLFOG1Sugrj4BFIhQGGnnNgBUSWG+bMU/J6sYsKmAlh2THSq2UeHGc+woq5ej5M9J3Ys1rie/+u1biOZWLEnu7JhMrmZxAJoMIEd0pIHRN91SVsbA3RYe3fQgROrlFIZO/7U6Yn0B1E0bR/nskTjT+2PA8CHHQpFIkEDvKJBksZC+ZgPGjAx6/TdSBHraryDPLcC+ci2SPDoxrD0hRFcoCOgyEOscCDdxtwti8RSisF1gcc5OGVdy84nYRGpkuo6l3c/Dlq4mYFAGYkfuNbuFPGhy7XrKhHvKI9KtDuJCxxNpxBv9G0OYZE6bRRvBFLxxqxXNHzZr/rjkBmIyYFZ3d3hb9L4G3O5RKTdoxgkrGXQZ/RyL3KDAWoDTkkN1xEefGcOPgiApCtOE2rO037zbcNrX5AVMJeEasmpS9KW5zjJJ0yQK5w9vgyTj1aN0mFFesy8kU7EzXbFToGZzbMhPhpLOYjVzROD4GcyzHnPfpXMfXQ50XwaM5OictTveqBL7PxSi9cb/LnbC4xNi+15xfF1ZFYkyMdZkCRZc+eCdWlH5sRBXzwhh6FMT67oQ1aeF2LpH1L/wdi2wgKRyrSQJCCD07fnGurXFeY75Uu9yqbNZQrNDRhbIyujlxOPg88HBI5jn681TnXcOrf+rei/QQVJXm7Bl7nl15abdG5+rWJTndEmSJpOeA5Zpw523NwT+bkR4wGz1P2j4xuM98OX1xnFb5mRNXwOcZSuLSt598enSAkdmsd2iOdAF0dBAsOOfUNOPFxvdNc3tXuABwy+NFJtMIDkijeFny+NqqDP8bImNXfFY+xfq1MYSUyY21AAAAABJRU5ErkJggg=='/>");
			extra_item.insertAfter(current);
			extra_item.on('click', function(e){
				WebWidget.save(sentence);
			});
		}
	});
})