### Пересечение автоматов
\
Первый автомат:
$A= \langle \sum_a, Q_a, s_a, T_a, \sigma_a \rangle$
\
$\sum_a= {a,b}$\
$Q_a = {1,2}$\
$s_a = {1}$\
$T_a = {1}$\
$\sigma_a = $![GraphA](Test1(A).svg)\
Второй автомат:
$B= \langle \sum_b, Q_a, s_b, T_b, \sigma_b \rangle$
\
$\sum_b= {a,b}$\
$Q_b = {1,2,3}$\
$s_b = {1}$\
$T_b = {1}$\
$\sigma_b = $![GraphB](Test1(B).svg)\
Пересечение автоматов:
\
$D= \langle \sum_d, Q_a, s_d, T_d, \sigma_d \rangle$
\
$\sum_d= {a,b}$\
$Q_d = {1,1,1,2,2,1,2,2,3,1,3,2}$\
$s_d = {1,1}$\
$T_d = {1,1}$\
$\sigma_d = $ | |a|b|$\lambda$ |
 | -- | -- | -- | -- | 
|$\langle 1,1 \rangle$|$\langle 1,2 \rangle$|$\langle 2,2 \rangle$|$\langle  \rangle$|
|$\langle 1,2 \rangle$|$\langle 1,2 \rangle$|$\langle 2,2 \rangle$|$\langle  \rangle$|
|$\langle 2,1 \rangle$|$\langle 1,2 \rangle$|$\langle 2,2 \rangle$|$\langle  \rangle$|
|$\langle 2,2 \rangle$|$\langle 1,2 \rangle$|$\langle 2,2 \rangle$|$\langle  \rangle$|
|$\langle 3,1 \rangle$|$\langle 1,2 \rangle$|$\langle 2,2 \rangle$|$\langle  \rangle$|
|$\langle 3,2 \rangle$|$\langle 1,2 \rangle$|$\langle 2,2 \rangle$|$\langle  \rangle$|

![GraphD](Test1(Result).svg)