# WatchCat. Requirements and Specifications document.

## Проект watchcat
OS: Linux
OS: Windows (experimental mode)
[скачать Watchcat_Release_1.05_win64.zip](https://github.com/Kvazikot/WatchCat/tags)

Проект на с++ использует вебкамеру
Для сборки нужен opencv и cmake.

Другие программы которые использует watchcat: ffplay и ffmpeg

python нужен чтобы запустить внешний скрипт send_email

подключить opencv

vsbaranov83@gmail.com Владимир Баранов

Дата создания: 13.03.21 14:52

## Основные требования (Requirements)
* r1. запись видео при срабатывании триггера
   аудио триггер (порог громкости превышает заданное значение)
   видео триггер (в кадре есть движение)
   речевой триггер( есть определенные слова или сочетания слов в речи )

* r2. Передача емайла при срабатывании тригера (посмотреть скрипт на пайтоне)

* r3. Включение сирены (сирена сначала будет мой голос запитченный на пару тонов для устрашения)

* r4. Идентификация по лицу для отмены тревоги. Лица членов семьи.

* r5. Уничтожение заголовка раздела 

* r6. Ведение статистики, например сколько за день проехало автомобилей. Сколько прошло людей.
* r7. Программа должна работать в фоновом режиме. Поэтому нужно рабить проект на две программы консольный и GUI.
      Не обязательно. Если иксы не сбрасывать а просто блокировать компьютер.
      Для интерфейса и управления можно использовать библиотеку highgui из поставки opencv.
* r8. Для проигрывания аудио можно использовать ffmpeg
* r9. Для проигрывания аудио в Windows можно использовать использовать фцнкцию PlaySound
* r10. Для проигрывания аудио в Linux можно использовать использовать gstreamer framework  
* r224. Большая часть софта должна быть реализована на Си++ с ускорением графических функций взависимости от hardware (cuda)
    * r224.1. Пайтон применяется для не реалтайм или функций не связанных с компьютерным зрением.
    * r224.2. Пайтон может применяться для создания быстрого прототипа приложения на opencv например [watchcat.py](https://github.com/Kvazikot/QuasiCode/blob/master/watchcat.py)
* r11. Использовать pipe для вывода видео из консольной программы через stdout в формате mjpeg
* r12. Использовать shared memory для вывода видео в GUI
* r14. Основа программы должны составлять функции фреймворка opencv потому что они отбкатаны во многих проектах, аппаратно ускоренны и т.д.
    * r14.1. Возможно некоторые части простые и быстрые функции watchcat не требуют многобайтных библиотек.      
    * r14.2. Использование opencv позволит унифицировать разработку и переиспользовать код проекта в других проектах.
     
* r15. Основа программы должна быть реализована на Qt фреймворке.
* r16. Требования к оператору. Технически образованный, знающий Пайтон.



## Программа Watchcat. Functional Requirements
* r221. Желательно чтобы watchcat принимал вход из сети. Например при трансляции видео со сматрфона.
        В линуксе это можно сделать используя gstreamer. <br>
        В windows можно принимать картинку используя qt. 
* r222. Желательно работа в тихом режиме (низкое потребление энергии)
         Исключает возможность использования ресурсозатратных триггеров.
* r223. Возможность выбирать тип триггеров



## Программа WatchcatGUI. Functional Specifications. GUI.
* s122. Программа с графическим интерфейсом на основе библиотеки с открытым кодом Qt 5.9.
* s123. WatchcatGUI взаимодействует с watchcat через shared memory.
 
## Программа WatchcatGUI. Interface Requirements. 
* r124. Программа должна иметь две вкладки. 
* r125. На первой вкладке идет изображение с камеры.
* r126. На первой вкладке кнопки включающие триггеры(движение в кадре) и алерты(звук, письмо на емайл).
* r127. На второй вкадке текст сообщения с алертом и список получателей.

## Программа Watchcat. Functional Specifications.
* s765. Использование shared memory. В консольной версии watchcat желательно использовать native API без привязки к QT.

##  Программа Watchcat. Performance Requirements. Ресурсозатратные Триггеры.
Используя dlib и обученную нейросеть можно добавить к текущей версии watchcat следующие триггеры:

* s825. триггер при появлениии в кадре лица
* s826. триггер при появлениии в кадре человека
* s827. триггер при появлениии в кадре авто
* s828. 
<br> 

##  Программа Watchcat. Performance Requirements. 
* r4390. Операционная система: Windows 2000 / Windows XP.
* r4391. Процессор (CPU): Intel Pentium III Mobile 750MHz / AMD-K6 3D.
* r4392. Видеокарта: GeForce 7600 GT / Radeon HD 7310G.
* r4393. Свободный объем оперативной памяти: 96 MB RAM.
* r4394. Свободное дисковое пространство: 500 MB Free.

## Программа Watchcat. Human factor Requirements.

* r829. Для распознавания сложных образов требуется машина с хорошей видеокартой.
    * r829.1 Это исключает спасение бедных детишек из Бангладеш от злых Терминаторов. Эхо Сталина все еще не дает мне спокойно спать.

## GUI Программа WatchcatGUI. Safery Requirements. Разблокировка по лицу.
* r1010. ИСпользовать код howdy. В линуксе для этого есть готовое решение - howdy.
<br>
<b>Недостатки</b>
<br>
* - Но работает через раз и требует настройки, много возни. Может полностью заблокировать графический интерфейс.
* - Неизвестны вопросы с лицензией.
* 



## Обработка аудио

Библиотека kfr оптимизирована для работы в реальном времени.
https://github.com/kfrlib/kfr/blob/master/examples/dft.cpp

## Обработка видео
Библиотека opencv 
Дополнительные библиотеки для motion tracking и идентификации по лицу.


## Использование watchcat в сматрфонах и rasbery py.
В качестве платформы можно использовать rasbery pi с дебианом.
Можно использовать андройд смартфон в качестве мозгового центра.
Можно помотреть какие продаются датчики движения.


### Программа Watchcat. Specifications. Алгоритм детекции движения:

1. Берется два последовательных кадра.
2. Берется серые компоненты
3. Гаусс сглаживания обоих кадров
4. Абсолютная разница между кадрами
5. Морфологический фильтр dilate
6. Поиск контуров
7. Нахождение максимального контура по площади
8. Срабатывание тревоги если таких контуров больше 3.


# Полезная литература
1. NASA Systems Engineering Handbook by National Aeronautics and Space Administration (NASA) 
2. Engineering Systems Meeting Human Needs in a Complex Technological World by Olivier L. de Weck, Daniel Roos, Christopher L. Magee, Charles M. Vest 
3. How to Think Like Leonardo da Vinci by Gelb, Michael J 
4. Systems Engineering Fundamentals and Applications by Reinhard Haberfellner, Olivier de Weck, Ernst Fricke, Siegfried Vössner 
5. SysML-v0.9-PDF-050110R1
6. OPCAT_Manual_3.0
7. Object-Process Methodology A Holistic Systems Paradigm by Professor Dov Dori (auth.)
8. How to Think Like Leonardo da Vinci by Gelb, Michael 
9. Engineering Systems Meeting Human Needs in a Complex Technological World by Olivier L. de Weck, Daniel Roos, Christopher L. Magee, Charles M. Vest
10. Systems Engineering Fundamentals and Applications by Reinhard Haberfellner, Olivier de Weck, Ernst Fricke, Siegfried Vössner 
11. NASA Systems Engineering Handbook by National Aeronautics and Space Administration (NASA) (z-lib.org).pdf











