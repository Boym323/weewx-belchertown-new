<!-- Rename this file to index_radar.inc 
See other example at https://github.com/poblabs/weewx-belchertown/issues/703 -->
<div id="tabs-container">
	<!-- Nav tabs -->
	<ul class="nav nav-tabs tabs-menu" role="tablist">
		<li role="presentation" class="active">
			<a href="#radar" aria-controls="radar" role="tab" data-toggle="tab">Radar</a>
		</li>
		<li role="presentation">
			<a href="#webcamZ" aria-controls="webcamZ" role="tab" data-toggle="tab">Kamera západ</a>
		</li>
	</ul>

	<!-- Tabs contents -->
	<div class="tab">
		<div id="radar" class="tab-content">
			<div class="radar_image">$radar_html</div>
		</div>
		<div id="webcamZ" class="tab-content">
			<a href="https://webcam.pocasi-loucka.eu/zapadni_kamera.jpg" target="_blank">
				<img src="https://webcam.pocasi-loucka.eu/zapadni_kamera_thumb.jpg" alt="Zapad">
			</a>
		</div>
	</div>

</div>

<script type="text/javascript">
jQuery(document).ready(function() {
    setInterval(reloadWebcams, 60000); // 1 mins in millis
    reloadWebcams(); // Initial load

    function reloadWebcams() {
        jQuery('#webcamZ img').each(function() {
            new_image_url = jQuery(this).attr('src').split('?')[0] + "?" + Math.floor(Math.random() * 999999999);
            jQuery(this).attr('src', new_image_url);
        });
    }
});

jQuery('.tabs-menu a').click(function(event) {
    event.preventDefault();

    // Toggle active class on tab buttons
    jQuery(this).parent().addClass("active");
    jQuery(this).parent().siblings().removeClass("active");

    // display only active tab content
    var activeTab = jQuery(this).attr("href");
    jQuery('.tab-content').not(activeTab).css("display","none");
    jQuery(activeTab).fadeIn();

});
</script>

<style>
.dark .tabs-menu > li > a {
    padding: 0px 5px;
}

.dark .nav-tabs>li.active>a, .dark .nav-tabs>li.active>a:focus, .dark .nav-tabs>li.active>a:hover {
    color: #e5554e;
}

.active a {
    /* color:red; */
}

.tab-content {
    display:none;
}

.tabs-menu > li > a {
    padding: 0px 5px;
}

#radar {
    display:block;
}

.toprow-height {
    min-height: 402px !important;
}
</style>