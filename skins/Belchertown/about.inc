<div class="entry-content about-outer" itemprop="text">
    <div class="row" style="margin-top:20px;">
        
        <!-- LEVÝ SLOUPEC: Mapa + Tabulka senzorů -->
        <div class="col-sm-4">
            <div class="about-map" style="margin-bottom: 15px; overflow: hidden; border-radius: 8px; border: 1px solid rgba(128,128,128,0.2);">
                <iframe style="border:none" src="https://frame.mapy.cz/s/luzacovupe" width="100%" height="280" frameborder="0"></iframe>
            </div>
            
            <div class="station-photo" style="margin-bottom: 20px;">
                <img src="../images/stanice.jpg" alt="Pocasi-loucka.eu" title="Pocasi-loucka.eu" class="img-responsive" style="border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1);">
            </div>

            <h4 style="text-align:center;">Stav senzorů</h4>
            <div class="table-responsive">
                <table class="table table-condensed" style="font-size:90%; border: 1px solid rgba(128,128,128,0.2);">
                    <thead>
                        <tr style="background: rgba(128,128,128,0.1);">
                            <th style="text-align:right; border-bottom: 2px solid rgba(128,128,128,0.3);">Senzor</th>
                            <th style="text-align:center; border-bottom: 2px solid rgba(128,128,128,0.3);">Baterie</th>
                            <th style="text-align:center; border-bottom: 2px solid rgba(128,128,128,0.3);">Signál (RSSI)</th>
                        </tr>
                    </thead>
                    <tbody>
                        
                        ## ---------------------------------------------
                        ## 1. ANEMOMETR (WS80)
                        ## ---------------------------------------------
                        <tr>
                            <td style="text-align:right; padding: 8px; vertical-align: middle;">Anemometr (WS80)</td>
                            <td style="text-align:center; vertical-align: middle;">
                                #if $current.ws80_batt.raw is not None
                                    ## Limit: pod 2.4V červená
                                    #if $current.ws80_batt.raw < 2.4
                                        <span style="color:#aa4444; font-weight:bold;">$current.ws80_batt.format("%.2f") </span>
                                    #else
                                        <span style="color:#44aa44;">$current.ws80_batt.format("%.2f") </span>
                                    #end if
                                #else
                                    <span style="color:#ccc;">N/A</span>
                                #end if
                            </td>
                            <td style="text-align:center; font-family: monospace; font-weight: bold; vertical-align: middle;">
                                #set $rssi = $current.ws80_rssi.raw
                                #if $rssi is not None
                                    #if $rssi >= -65
                                        <span title="Výborný ($rssi dBm)" style="color:#44aa44; font-size:1.2em;">▂▃▅▇</span>
                                    #elif $rssi >= -75
                                        <span title="Dobrý ($rssi dBm)" style="color:#99cc00; font-size:1.2em;">▂▃▅</span>
                                    #elif $rssi >= -85
                                        <span title="Dostatečný ($rssi dBm)" style="color:#F96B00; font-size:1.2em;">▂▃</span>
                                    #else
                                        <span title="Slabý ($rssi dBm)" style="color:#aa4444; font-size:1.2em;">⚠️</span>
                                    #end if
                                #else
                                    <span style="color:#ccc;">?</span>
                                #end if
                            </td>
                        </tr>
                        
                        ## ---------------------------------------------
                        ## 2. PŮDNÍ VLHKOST (WH51)
                        ## ---------------------------------------------
                        <tr>
                            <td style="text-align:right; padding: 8px; vertical-align: middle;">Půdní vlhkost</td>
                            <td style="text-align:center; vertical-align: middle;">
                                #if $current.soilMoistBatt1.raw is not None
                                    ## 1x AA baterie -> Limit pod 1.2V
                                    #if $current.soilMoistBatt1.raw < 1.2
                                        <span style="color:#aa4444; font-weight:bold;">$current.soilMoistBatt1.format("%.2f") </span>
                                    #else
                                        <span style="color:#44aa44;">$current.soilMoistBatt1.format("%.2f") </span>
                                    #end if
                                #else
                                    <span style="color:#ccc;">N/A</span>
                                #end if
                            </td>
                            <td style="text-align:center; font-family: monospace; font-weight: bold; vertical-align: middle;">
                                #set $rssi = $current.wh51_ch1_rssi.raw
                                #if $rssi is not None
                                    #if $rssi >= -65
                                        <span title="Výborný ($rssi dBm)" style="color:#44aa44; font-size:1.2em;">▂▃▅▇</span>
                                    #elif $rssi >= -75
                                        <span title="Dobrý ($rssi dBm)" style="color:#99cc00; font-size:1.2em;">▂▃▅</span>
                                    #elif $rssi >= -85
                                        <span title="Dostatečný ($rssi dBm)" style="color:#F96B00; font-size:1.2em;">▂▃</span>
                                    #else
                                        <span title="Slabý ($rssi dBm)" style="color:#aa4444; font-size:1.2em;">⚠️</span>
                                    #end if
                                #else
                                    <span style="color:#ccc;">?</span>
                                #end if
                            </td>
                        </tr>

                        ## ---------------------------------------------
                        ## 3. BLESKOMĚR (WH57)
                        ## ---------------------------------------------
                        <tr>
                            <td style="text-align:right; padding: 8px; vertical-align: middle;">Bleskoměr</td>
                            <td style="text-align:center; vertical-align: middle;">
                                #if $current.lightning_Batt.raw is not None
                                    ## Detekce: pokud je hodnota > 2, bere to jako volty, jinak jako status
                                    #if $current.lightning_Batt.raw > 2
                                        #if $current.lightning_Batt.raw < 2.4
                                            <span style="color:#aa4444; font-weight:bold;">$current.lightning_Batt.format("%.2f") V</span>
                                        #else
                                            <span style="color:#44aa44;">$current.lightning_Batt.format("%.2f") V</span>
                                        #end if
                                    #else
                                        ## Starší driver může posílat jen 0/1 (flag)
                                        #if $current.lightning_Batt.raw == 1
                                            <span style="color:#aa4444;">Nízká</span>
                                        #else
                                            <span style="color:#44aa44;">OK</span>
                                        #end if
                                    #end if
                                #else
                                    <span style="color:#ccc;">N/A</span>
                                #end if
                            </td>
                            <td style="text-align:center; font-family: monospace; font-weight: bold; vertical-align: middle;">
                                #set $rssi = $current.wh57_rssi.raw
                                #if $rssi is not None
                                    #if $rssi >= -65
                                        <span title="Výborný ($rssi dBm)" style="color:#44aa44; font-size:1.2em;">▂▃▅▇</span>
                                    #elif $rssi >= -75
                                        <span title="Dobrý ($rssi dBm)" style="color:#99cc00; font-size:1.2em;">▂▃▅</span>
                                    #elif $rssi >= -85
                                        <span title="Dostatečný ($rssi dBm)" style="color:#F96B00; font-size:1.2em;">▂▃</span>
                                    #else
                                        <span title="Slabý ($rssi dBm)" style="color:#aa4444; font-size:1.2em;">⚠️</span>
                                    #end if
                                #else
                                    <span style="color:#ccc;">?</span>
                                #end if
                            </td>
                        </tr>

                        ## ---------------------------------------------
                        ## 4. KVALITA OVZDUŠÍ (PM2.5 / WH41)
                        ## ---------------------------------------------
                        <tr>
                            <td style="text-align:right; padding: 8px; vertical-align: middle;">Měření PM<sub>2,5</sub></td>
                            <td style="text-align:center; vertical-align: middle;">
                                #if $current.pm25_Batt1.raw is not None
                                    ## Interní Li-ion baterie -> Limit < 3.6V
                                    #if $current.pm25_Batt1.raw > 2
                                        #if $current.pm25_Batt1.raw < 3.6
                                            <span style="color:#aa4444; font-weight:bold;">$current.pm25_Batt1.format("%.2f") V</span>
                                        #else
                                            <span style="color:#44aa44;">$current.pm25_Batt1.format("%.2f") V</span>
                                        #end if
                                    #else
                                        #if $current.pm25_Batt1.raw == 1
                                            <span style="color:#aa4444;">Low</span>
                                        #else
                                            <span style="color:#44aa44;">OK</span>
                                        #end if
                                    #end if
                                #else
                                    <span style="color:#ccc;">N/A</span>
                                #end if
                            </td>
                            <td style="text-align:center; font-family: monospace; font-weight: bold; vertical-align: middle;">
                                #set $rssi = $current.wh41_ch1_rssi.raw
                                #if $rssi is not None
                                    #if $rssi >= -65
                                        <span title="Výborný ($rssi dBm)" style="color:#44aa44; font-size:1.2em;">▂▃▅▇</span>
                                    #elif $rssi >= -75
                                        <span title="Dobrý ($rssi dBm)" style="color:#99cc00; font-size:1.2em;">▂▃▅</span>
                                    #elif $rssi >= -85
                                        <span title="Dostatečný ($rssi dBm)" style="color:#F96B00; font-size:1.2em;">▂▃</span>
                                    #else
                                        <span title="Slabý ($rssi dBm)" style="color:#aa4444; font-size:1.2em;">⚠️</span>
                                    #end if
                                #else
                                    <span style="color:#ccc;">?</span>
                                #end if
                            </td>
                        </tr>
                    </tbody>
                </table>
            </div>
        </div>

        <!-- PRAVÝ SLOUPEC: Text -->
        <div class="col-sm-8" style="padding-left:30px;">
            <h3>O stanici Loučka</h3>
            <p>
                Meteostanice se nachází v obci <a href="https://www.loucka-obec.cz/" target="_blank">Loučka</a> (Zlínský kraj) na okraji <strong>Vizovických vrchů</strong>. 
                Tato oblast leží v mírném klimatickém pásu, kde léta bývají teplá (průměrně 25°C) a zimy mrazivé (průměrně -5°C).
            </p>
            
            <div class="alert alert-info" style="font-size: 0.9em; background-color: rgba(0, 123, 255, 0.1); border-color: rgba(0, 123, 255, 0.2); color: #0056b3;">
                Data jsou aktualizována každých <strong>20 sekund</strong>. Archivace probíhá v 5minutových intervalech.
            </div>

            <h4>Historie a milníky</h4>
            <ul class="list-unstyled" style="line-height: 1.6;">
                <li><span class="label label-default" style="display:inline-block; width:90px;">2024-09-21</span> Přemístění a výměna <a href="../images/stit_2024.jpg" target="_blank">radiačního štítu</a>.</li>
                <li><span class="label label-default" style="display:inline-block; width:90px;">2023-11-09</span> Ukončení provozu východní kamery po 10 letech.</li>
                <li><span class="label label-default" style="display:inline-block; width:90px;">2023-09-17</span> Nová západní kamera Dahua HFW2241S.</li>
                <li><span class="label label-default" style="display:inline-block; width:90px;">2022-08-20</span> Přechod na gateway Ecowitt GW1000.</li>
                <li><span class="label label-default" style="display:inline-block; width:90px;">2018-11-28</span> Měření přízemní teploty a půdních čidel (-100 cm).</li>
                <li><span class="label label-default" style="display:inline-block; width:90px;">2009-03-29</span> <strong>Zahájení měření</strong> se stanicí Watson W-8681.</li>
            </ul>

            <h4 style="margin-top:25px;">Použitý hardware</h4>
            <div class="row">
                <div class="col-md-6">
                    <ul style="font-size: 0.95em;">
                        <li><strong>Venkovní teplota:</strong> Ecowitt WH32</li>
                        <li><strong>Vítr a UV:</strong> Ecowitt WS80 (Ultrasonic)</li>
                        <li><strong>Srážkoměr:</strong> Ecowitt WH40</li>
                    </ul>
                </div>
                <div class="col-md-6">
                    <ul style="font-size: 0.95em;">
                        <li><strong>Kvalita ovzduší:</strong> Ecowitt WH41 (PM2.5)</li>
                        <li><strong>Blesky:</strong> Ecowitt WH57</li>
                        <li><strong>Půda:</strong> WH51 a DS18B20</li>
                    </ul>
                </div>
            </div>

            <p style="margin-top:20px;">
                Sledujte nás také na: 
                <a class="btn btn-xs btn-primary" href="http://www.wunderground.com/personal-weather-station/dashboard?ID=IZLNSKLO2" target="_blank">Weather Underground</a>
                <a class="btn btn-xs btn-info" href="https://www.pwsweather.com/station/pws/LOUCKA" target="_blank">PWSWeather</a>
            </p>
        </div>
    </div>
</div>