// Author: @steve-quezada
// Script para Desmos: dibujo del gato usando coordenadas del modelo
(function() {
  Calc.setBlank();

  Calc.updateSettings({
    showGrid: false,
    showXAxis: false,
    showYAxis: false
  });
  
  Calc.setMathBounds({ left: -60, right: 60, bottom: -140, top: 140 });

  const NEGRO = '#000000';
  const BLANCO = '#FFFFFF';
  const ROSA = '#DC6482';
  const OSCURO = '#3C1414';

  let expresiones = [];
  let contador = 1;

  function dibujar(latex, color, opciones_extra = {}) {
    expresiones.push(Object.assign({ 
      id: 'michi_' + contador++, 
      latex: latex, 
      color: color 
    }, opciones_extra));
  }

  // A. Contorno principal (polilínea cerrada)
  dibujar('\\left[(-41,-121), (-38,-91), (-34,-60), (-30,-39), (-29,-30), (-38,-11), (-43,6), (-44,24), (-41,48), (-36,67), (-31,91), (-27,115), (-26,118), (-19,102), (-12,94), (-6,97), (0,99), (6,97), (13,92), (21,102), (27,118), (32,91), (36,67), (41,48), (46,24), (46,6), (42,-11), (31,-33), (34,-54), (38,-91), (42,-121), (-41,-121)\\right]', NEGRO, {lines: true, points: false, lineWidth: 4});

  // B. Fondo oscuro de la boca (polígono relleno)
  dibujar('\\operatorname{polygon}((-11,17), (-8,2), (-4,-5), (0,-7), (4,-5), (8,2), (11,17), (10,13), (8,10), (6,9), (5,9), (3,11), (2,13), (1,14), (0,15), (-1,14), (-2,13), (-3,11), (-5,9), (-6,9), (-8,10), (-10,13))', OSCURO, {fillOpacity: 1, lines: false});

  // C. Lengua (relleno) y borde (línea cerrada)
  dibujar('\\operatorname{polygon}((-6,2), (6,2), (6,-12), (5,-15), (3,-17), (0,-18), (-3,-17), (-5,-15), (-6,-12))', ROSA, {fillOpacity: 1, lines: false});
  dibujar('\\left[(-6,2), (6,2), (6,-12), (5,-15), (3,-17), (0,-18), (-3,-17), (-5,-15), (-6,-12), (-6,2)\\right]', NEGRO, {lines: true, points: false, lineWidth: 3});

  // D. Labio superior (trazo)
  dibujar('\\left[(-11,17), (-10,13), (-8,10), (-6,9), (-5,9), (-3,11), (-2,13), (-1,14), (0,15), (1,14), (2,13), (3,11), (5,9), (6,9), (8,10), (10,13), (11,17)\\right]', NEGRO, {lines: true, points: false, lineWidth: 4});

  // E. Ojos (elipses aproximadas)
  dibujar('\\frac{(x+18)^2}{32} + \\frac{(y-35)^2}{105} \\le 1', NEGRO, {fillOpacity: 1, lines: false});
  dibujar('\\frac{(x-18)^2}{32} + \\frac{(y-35)^2}{105} \\le 1', NEGRO, {fillOpacity: 1, lines: false});
  dibujar('\\frac{(x+20.149575)^2}{1.402333} + \\frac{(y-38.893741)^2}{5.609333} \\le 1', BLANCO, {fillOpacity: 1, lines: false});
  dibujar('\\frac{(x-15.850425)^2}{1.402333} + \\frac{(y-38.893741)^2}{5.609333} \\le 1', BLANCO, {fillOpacity: 1, lines: false});

  // G. Cejas (segmentos)
  dibujar('\\left[(-29,57), (-19,49), (-10,40)\\right]', NEGRO, {lines: true, points: false, lineWidth: 4});
  dibujar('\\left[(29,57), (19,49), (10,40)\\right]', NEGRO, {lines: true, points: false, lineWidth: 4});

  // H. Líneas de movimiento
  dibujar('\\left[(-19,-14), (-11,-14)\\right]', NEGRO, {lines: true, points: false, lineWidth: 3});
  dibujar('\\left[(-22,-22), (-13,-22)\\right]', NEGRO, {lines: true, points: false, lineWidth: 3});
  dibujar('\\left[(-18,-30), (-10,-30)\\right]', NEGRO, {lines: true, points: false, lineWidth: 3});

  // --- INYECCIÓN FINAL ---
  Calc.setExpressions(expresiones);
  console.log("Desmos: Michi cargado.");
})();
