# MODULES
You can buy the plugin here - https://discord.com/invite/g798xERK5Y
Купить плагин можно тут - https://discord.com/invite/g798xERK5Y

- LR Progress<br>
Event when gaining experience<br>
Ивент который срабатывает при получении опыта
```
event - lr_exp
int param - count
```

- Shop Progress<br>
Event when receiving currency<br>
Ивент который срабатывает при получении какой либо валюты
```
event - shop_give_currency
string param - currency
int param - count
```
Event when currency is spent<br>
Ивент которые срабатывает при трате какой-либо валюты
```
event - shop_take_currency
string param - currency
int param - count
```

- Play Time<br>
An event triggered every minute<br>
Ивент вызываемый каждую минуту
```
event - playtime
int param - team
``` 

- VIP FREE PASS<br>
Allows VIP players to use a paid battle pass during their active VIP status<br>
Позволяет VIP-игрокам использовать платный боевой пропуск во время действия VIP-статуса<br>
```
groups.ini: 
"bp_pass"   "1"
```

- Shop LR [BY PISEX]<br>
Добавляет в магазин внутри плагина возможность покупать опыт<br>

- Shop Rcon<br>
Добавляет в магазин внутри плагина возможность покупать RCON команды, возможные теги внутри команды - {userid}, {steamid64}<br>

- Shop Shop [BY PISEX]<br>
Добавляет в магазин внутри плагина возможность покупать разные валюты и предметы для !shop<br>

- Shop VIP [BY PISEX]<br>
Добавляет в магазин внутри плагина возможность покупать вип статус<br>