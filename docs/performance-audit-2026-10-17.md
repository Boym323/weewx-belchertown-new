# Audit a první optimalizační průchod – 17. 10. 2026

**Repozitář:** Boym323/weewx-belchertown-new · **základ:** `master` (`f7fd8e0c8542`)  
**Rozsah:** statická analýza zdrojového kódu, regresní testy bez WeeWX.  
**Neprovedeno:** profilování produkčního WeeWX, měření v prohlížeči, průchod API dodavatelů, migrace konfigurace na cílovém hostu, produkční deploy. **Žádné číselné zrychlení tímto auditem netvrdíme.**

## Potvrzené příležitosti a provedené změny

| Priorita | Místo | Původní stav | Změna | Ověření |
| --- | --- | --- | --- | --- |
| P0 | `header.html.tmpl` | CSS a Cheetah-generovaný JS dostávaly při každém reportu nový `time.time()` query parametr. | Stabilní `asset_version` jen pro statické CSS; JS dál zůstává reportově verzovaný, protože obsahuje aktuální časové údaje. | Test ověřuje čtyři CSS odkazy a zachování verze generovaného JS. |
| P1 | `header.html.tmpl` + `belchertown.js.tmpl` | Všechny stránky kromě kiosku načítaly šest modulů Highcharts; globální `setOptions` znemožňoval prosté vypnutí. | Moduly jen na stránce grafů a na domovské stránce s grafy, chráněná inicializace. | Node test se zapnutým i vypnutým Highcharts. |
| P1 | `header.html.tmpl` | Kvůli prioritě operátorů se Paho načítal na `home` a `pi` i při vypnutém MQTT. | Opraveno seskupení podmínek. | Statický regresní test. |
| P1 | `belchertown.js.tmpl` | `onMessageArrived` a `update_current_wx` parsovaly tentýž MQTT payload. | Jediné parsování na zprávu; `update_current_wx` dál přijímá i JSON řetězec. | Node test počítá volání parseru a testuje neplatný JSON. |
| P1 | `belchertown.py` | Ve skupinové smyčce vznikal opakovaný `graphs.json` i bez změny konfigurace. | Jeden přehledný zápis mimo smyčku, pouze pokud se obsah liší. | Strukturální Python test. |
| P1 | `belchertown.py` | Generované JSON grafů bylo pretty-printed s odsazením 4. | Kompaktní JSON zapisovaný přímo do souboru. | Python test sémantické parity na syntetickém vzorku. |

## Provozní pravidla

1. **Před nasazením:** zaznamenat aktuální SHA, zazálohovat soubory v `/etc/weewx/skins/Belchertown` a `/etc/weewx/bin/user`, provést kontrolu přepsání lokálního `skin.conf`.
2. **Cache statických souborů:** při změně `*.css` zvýšit `[Extras] asset_version`. Generovaný `js/belchertown.js.tmpl` obsahuje i aktuální údaje (`$current.dateTime.raw`), proto zatím ponecháváme původní časový parametr na každé vygenerování reportu. JS lze dlouhodobě cachovat až po oddělení dynamické konfigurace. V produkci ověřit cache-hit rate pro CSS.
3. **Regrese:** ověřit desktop, mobil, kiosk, `/pi`, `/graphs`, `/records`, režimy MQTT zapnuto/vypnuto, světlý/tmavý motiv a webovou konzoli.
4. **Grafy:** ověřit přesnou shodu datových bodů starého/nového JSON (`JSON.parse`), nezměněný časový rozsah a fungování exportů; nová kompaktní data již nejsou snadno čitelná prostým otevřením souboru.
5. **Rollback:** revert celého PR a nové vygenerování WeeWX reportu; případně obnovit zálohované soubory. Nesahat na archivní databázi ani na MQTT broker.

## Další doporučené kroky (zatím neimplementované)

| Priorita | Hypotéza / riziko | Potřebné důkazy |
| --- | --- | --- |
| P0 | `.github/workflows/deploy.yml` používá sdílený self-hosted runner, `git pull origin master` bez připnutí ke `GITHUB_SHA` a `rsync --delete` při kopírování skinu. Část kopií není atomická. | Ověřit lokální úpravy, vlastnictví souborů a cílové cesty. Následně samostatný PR: pinned commit, staging, dry-run, backup, validace, rollback. |
| P1 | `HighchartsJsonGenerator.get_observation_data` volá opakovaně `weewx.xtypes.get_series` pro více křivek a agregací. Potenciální hlavní zátěž DB/CPU. | Měření `time.perf_counter` po křivkách, počty dotazů, `EXPLAIN` v SQLite/MySQL dle produkce, 24h/7d data. |
| P1 | `getData.get_extension_list` dělá souhrnné statistiky, přístupy k DB a volitelné vzdálené weather/earthquake API. | Čas po fázích; síťové latence, timeouty, objem IO, aktualizační intervaly, vypnutí nepotřebných zdrojů. |
| P1 | `js/highcharts/highstock.js` je přes 400 kB a další moduly mají dohromady stovky kB. | HAR / Lighthouse na mobilu, CPU profiler, komprese HTTP, podíl návštěv `/graphs`. |
| P1 | `belchertown.js.tmpl` obsahuje řadu DOM operací na každou MQTT aktualizaci a využívá mnoho implicitních globálních proměnných. | Profilování zpráv za sekundu, `PerformanceObserver`, long tasks, heap; poté dávkování změn bez zpoždění kritických hodnot. |
| P2 | Bootstrap 3.4.1, jQuery 3.3.1, Moment 2.24 a část CDN závislostí jsou starší. | Kontrola kompatibility, bezpečnostní audit, samostatný migrační plán + vizuální snapshoty všech stránek. |
| P2 | Současný balík nemá plné integrační testy WeeWX / Cheetah. | Zřídit izolované WeeWX 5 prostředí s fixture databází a srovnání produkovaných stránek a grafů. |

## Měřicí plán pro produkci

- **Baseline 24 h:** čas `weectl report run` (P50/P95), CPU a RSS procesu, počet/zatížení SQL dotazů, počty zápisů a bajtů do `HTML_ROOT/json`, počet stažení assetů.
- **Profil prohlížeče:** 3× cold-cache a 3× warm-cache na desktopu a mobilu; TTFB/FCP/LCP, transferred bytes, dlouhé úlohy, počet MQTT zpráv a jejich zpracování, průměrná a maximální doba renderu grafů.
- **A/B:** stejný rozsah historických dat, stejné intervaly reportů, stejné prohlížeče; porovnávat medián i P95.
- **Stop kritéria:** ztracené MQTT údaje, jiné hodnoty grafů, poškozené HTML, nefunkční kiosk/PI, prodloužení reportu >10 % nebo nárůst RSS >10 % oproti baseline.

Offline testy:
```sh
python3 -m py_compile bin/user/belchertown.py bin/user/tablegenerator.py install.py
python3 -m unittest discover -s tests -p 'test_*.py' -v
node --test tests/js/*.test.mjs
```
