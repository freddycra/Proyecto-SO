# Proyecto-SO
Proyecto 1 de Sistemas Operativos, crear un bash.


El proyecto consiste en diseñar un programa en C que sirva como una interfaz de línea de comandos (Shell), el cual acepta comandos de usuario y luego ejecuta cada comando en un proceso separado. Éste proyecto debe ser desarrollado en un ambiente Linux.

La interfaz de Shell proporciona un "prompt" al usuario, después del cual un comando es ingresado. El ejemplo muestra el prompt tiger> y el comando proporcionado por el usuario cat ejemplo.c

            tiger> cat ejemplo.c

Una técnica para implementar la interfaz Shell es permitirle al proceso padre leer lo que el usuario digita en la línea de comandos (cat ejemplo.c) y luego crear un proceso hijo separado que se encarga de la ejecución del comando. A menos de que se especifique de otra manera, el proceso padre espera a que el proceso hijo termine la ejecución del comando antes de continuar. Sin embargo, shells de UNIX también permiten que el proceso hijo se ejecute en segundo plano, o concurrentemente. Para lograr ésto, se agrega un '&' al final del comando. Por ejemplo:

            tiger> cat ejemplo.c &

El proceso hijo es creado utilizando la llamada al sistema fork(), y el comando ingresado por el usuario se ejecuta con la llamada al sistema exec() - tome en cuenta que exec() tiene múltiples tipos de llamadas, consulte el manual de exec() para obtener más información.

Además de la funcionalidad descrita, el proyecto debe implementar un historial de comandos, que le permita al usuario acceder a los comandos que se introdujeron más recientemente. El usuario debe ser capaz de acceder hasta un máximo de 10 comandos en el historial. Los comandos deben ser numerados de manera secuencial iniciando en 1, y la secuencia de números debe seguir incrementándose incluso si se sobrepasan los 10 comandos. Por ejemplo, si el usuario digitó 35 comandos, los últimos 10 comandos estarán numerados del 26 al 35. 

El usuario deberá poder ver el historial digitando el comando historial desde el prompt. A manera de ejemplo, suponga que el usuario digitó los siguientes comandos (de más a menos reciente):

    ps, ls -l, top, cal, who, date

El historial mostrará lo siguiente:

    6 ps

    5 ls -l

    4 top

    3 cal

    2 who

    1 date

El programa debe técnicas para obtener comandos desde el historial:

- Cuando el usuario digita el signo de admiración ! seguido de un número N, el enésimo comando del historial es devuelto. Por ejemplo, !3 devolvería el tercer comando existente en el historial.

Note que debe manejar situaciones de error (como por ejemplo, si no existen comandos en el historial, o si no se proporciona un número con el signo de admiración !)

Finalmente, investigue cómo instalar el shell de manera que esté disponible en un sistema linux, así si un usuario no desea utilizar bash, que utilice el shell que ustedes crearon por defecto.

Consulte más información para éste proyecto en el capítulo 3 del libro Operating System Concepts, 9th edition.



Notas:

El proyecto se puede realizar en grupos de máximo 2 personas
Fecha de entrega: Jueves 22 de marzo, en horas de clase
Entregar los archivos de código fuente, junto con los pasos para compilar el código
