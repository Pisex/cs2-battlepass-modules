# MODULES
- LR Progress
Event when gaining experience
Ивент который срабатывает при получении опыта
```
event - lr_exp
int param - count
```

- Shop Progress
Event when receiving currency
Ивент который срабатывает при получении какой либо валюты
```
event - shop_give_currency
string param - currency
int param - count
```
Event when currency is spent
Ивент которые срабатывает при трате какой-либо валюты
```
event - shop_take_currency
string param - currency
int param - count
```

- Play Time
An event triggered every minute
Ивент вызываемый каждую минуту
```
event - playtime
int param - team
``` 

- VIP FREE PASS
Allows VIP players to use a paid battle pass during their active VIP status
Позволяет VIP-игрокам использовать платный боевой пропуск во время действия VIP-статуса
```
groups.ini: 
"bp_pass"   "1"
```