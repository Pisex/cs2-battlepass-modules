# MODULES
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

- Play Time
An event triggered every minute<br>
Ивент вызываемый каждую минуту<br>
```
event - playtime
int param - team
``` 

- VIP FREE PASS
Allows VIP players to use a paid battle pass during their active VIP status<br>
Позволяет VIP-игрокам использовать платный боевой пропуск во время действия VIP-статуса<br>
```
groups.ini: 
"bp_pass"   "1"
```