<script type="text/javascript">
    var moment_locale = "$system_locale_js";
    moment.locale(moment_locale);

    jQuery(document).ready(function() {

      jQuery("#year-pm2_5-max-ts").text( moment.unix($year.pm25_1.maxtime.raw).format( "$obs.label.time_records_page_full_date" ) );
      jQuery("#at-pm2_5-max-ts").text( moment.unix($alltime.pm25_1.maxtime.raw).format( "$obs.label.time_records_page_full_date" ) );

      jQuery("#year-lightning_strike_count-sum-ts").text( moment.unix($year.lightning_strike_count.maxtime.raw).format( "$obs.label.time_records_page_full_date" ) );
      jQuery("#at-lightning_strike_count-sum-ts").text( moment.unix($alltime.lightning_strike_count.maxtime.raw).format( "$obs.label.time_records_page_full_date" ) );


    });
</script>



            <tr>
                <td class="records-header" colspan=5>$obs.label.records_air</td>
            </tr>
            <tr>
                <td colspan=1> </td>
                <td class="records-header-year" colspan=2>$current.dateTime.format("%Y")</td>
                <td class="records-header-alltime" colspan=2>$obs.label.records_all_time</td>
            </tr>
            <tr>
                <td>$obs.label.records_air</td>
                <td class="leftborder">$year.pm25_1.max</td>
                <td><span id="year-pm2_5-max-ts"></span></td><!-- moment.js -->
                <td>$alltime.pm25_1.max</td>
                <td class="leftborder"><span id="at-pm2_5-max-ts"></span></td><!-- moment.js -->
            </tr>
           
            <tr>
                <td>$obs.label.records_lightning</td>
                <td class="leftborder">$year.lightning_strike_count.maxsum</td>
                <td id="year-lightning_strike_count-sum-ts"></td> 
                <td>$alltime.lightning_strike_count.maxsum</td>
                <td class="leftborder" id="at-lightning_strike_count-sum-ts"></td>
            </tr>
    