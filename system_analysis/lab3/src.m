clear all;
clc;
close all;

1;
function y = func(x)
  y = 2*sin(x);
endfunction

% func(2)
% func(1:5:100)

function f = systema(~, x)
  global C;
  f(1) = x(2);
  f(2) = x(3);
  f(3) = (C(1) + C(2)*x(1) + C(3)*x(2) + C(4)*x(3) + C(5)*x(1)*x(2) ...
  + C(6)*x(2)*x(3) + C(7)*x(1)*x(3) + C(8)*x(1)*x(1) ...
  + C(9)*x(2)*x(2) + C(10)*x(3)*x(3) + C(11)*x(1)*x(2)*x(3) ...
  + C(12)*x(1)*x(1)*x(2) + C(13)*x(1)*x(1)*x(3) ...
  + C(14)*x(1)*x(2)*x(2) + C(15)*x(2)*x(2)*x(3) + C(16)*x(1)*x(3)*x(3) ...
  + C(17)*x(2)*x(3)*x(3) + C(18)*x(1)*x(1)*x(1) ...
  + C(19)*x(2)*x(2)*x(2) + C(20)*x(3)*x(3)*x(3));
  f = f';
endfunction

% формирование необходимых временных рядов
% границы отрезка
a = 0;
b = 5;
global C x1 x2 x3 x4;
n = 500;
m = 20;
% шаг интегрирования
step = (b - a) / n;
% временная ось
x = a:step:b;
% значения исходной функции

y = func(x);
y2 = zeros(1,n+1);
y3 = zeros(1,n+1);
y4 = zeros(1,n+1);
x1 = zeros(m,1);
x2 = zeros(m,1);
x3 = zeros(m,1);
x4 = zeros(m,1);
C = zeros(1,m);
% Вычиследние производных
for i=1:n-1
  y2(i)=(y(i+1)-y(i))/step;
end
for i=1:n-1
  y3(i)=(y2(i+1)-y2(i))/step;
end
for i=1:n-1
  y4(i)=(y3(i+1)-y3(i))/step;
end
% выборочные точки
for i=0:m-1
  x1(i+1)=y(round(n/m)*i+1);
  x2(i+1)=y2(round(n/m)*i+1);
  x3(i+1)=y3(round(n/m)*i+1);
  x4(i+1)=y4(round(n/m)*i+1);
end

% формирование нелинейной функции
A = zeros(m,m);
for i=1:m
  A(i,:) = [1 x1(i) x2(i) x3(i) x1(i)*x2(i) ...
    x2(i)*x3(i) x1(i)*x3(i) (x1(i))^2 (x2(i))^2 ...
    (x3(i))^2 x1(i)*x2(i)*x3(i) (x1(i))^2*x2(i) ...
    (x1(i))^2*x3(i) x1(i)*(x2(i))^2 (x2(i))^2*x3(i) ...
    x1(i)*(x3(i))^2 x2(i)*(x3(i))^2 (x1(i))^3 ...
    (x2(i))^3 (x3(i))^3];
end

% находение коэф. Ci и решение системы дифф. уравнений

C = A\x4;
disp('C = ');
disp(C);
[~, s] = ode45('systema', x, [x1(1) x2(1) x3(1)]);
disp('The solve is ');
disp(s);
% визуализация результатов
Y = s(:,1);
Y2 = s(:,2);

% === PATCH =============

y = y(1:1:length(Y))
x = x(1:1:length(Y))
Y2 = Y2(1:1:length(Y))
y2 = y2(1:1:length(Y))

% =======================

figure;
plot(x,y,'-b',x,Y,'-r');
grid on
title('График моделируемой и оригинальной функций');
legend('оригинальная функция','моделируемая функция', 'location', 'northeast')
figure;
plot(y,y2, '-b',Y,Y2,'-r')
grid on
title('Фазовые портреты');
legend('оригинальная функция','моделируемая функция', 'location', 'northeast')