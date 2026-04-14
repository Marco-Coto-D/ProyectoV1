Ideas de logica para el proyecto (para Fabian)

La clase equipo ocupa de la clase incidencias 

La clase Incidencia seria la clase padre de tres clases diferentes (Incidencia baja, incidencia media, incidencia alta)

bool activa (para indicar si la incidencia esta activa para los diferentes equipos, esto se planea aplicar con un rand aleatoriamente)

Metodos

virtual puro --> int convertirSeveridad() (Este metodo cambiaria el string de severidad a un valor int para aplicar a la formula de prioridad)

virtual puro --> void simularIncidencia() (Este metodo es el encargado de activar aleatoriamente una incidencia a los equipos de la simulacion, ademas de la severidad del mismo)

La clase mantenimiento

solo deberia tener el metodo void aplicar() (Este metodo se encarga de mejorar el estado del equipo , ademas limpia todas sus incidencias asignandolas a false)

Por ultimo la clase Sistema

### Todos los metodos de esta clase se usaran mediante un menu mostrado en el main.

Sus atributos serian

vectores de incidencias y equipos

Cuenta de dias para la simulacion

Metodos:

void simular() (Este metodo va a simular los 30 dias pero con una validacion para avanzar de dia y que no se simulen los 30 dias de un solo)

void simularDia() (Este metodo va a usarse dentro del metodo simular ya que cada dia se va a simular con la logica de este metodo)

void ordenarEquipos() (Este metodo sera el encargado de ordenar los equipos por su prioridad de mantenimiento facilitando la eleccion de dichos equipos)

void seleccionarEquipos() (Este metodo va a seleccionar los 3 equipos del lab con mayor riesgo)

void aplicarMantenimiento() (Este metodo va a usar el metodo seleccionarEquipos() y aplicarles dicho mantenimiento)