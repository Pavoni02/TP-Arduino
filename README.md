# Estación meteorológica  
  
Videos  
https://drive.google.com/drive/folders/1a60Re3I-AZrzL9qCcXft5rEZTI-9KE0e?usp=sharing  
  
TinkerCAD inicial  
https://www.tinkercad.com/things/b2gM4IPGW7K/editel?sharecode=kH8NKpH_HSYiA50epHQZfdeHs1__0OeczNNd7epc79w  
  
# Descripción  
  
Se miden variables climatológicas para determinar temperatura, humedad, lluvia y nivel de luz   
Las variables se actualizan cada 90 segundos en forma automática, con el pulsador o con escrituras bluetooth se actualizan inmediatamente (mediciones forzadas)   
Las mismas se mostraran por un display IC2 y también por Bluetooth  
Se realizan alertas de temperatura (Led RGB), de humedad (Led 1) y de nivel de luz (Led 2). Además en el display se muestran las variables de temperatura, humedad y lluvia con sus respectivas alertas.    
   
El umbral del led LDR es desde 0 a 650 y es inverso a la cantidad de luz presente en el ambiente 
El umbral del led de humedad es desde 58% a 100% y es inverso a la humedad del ambiente
  
# Componentes  
  
DHT-11 (Digital)  
HC-05 (Digital)  
Display I2C (Digital)  
Pulsador  
Led RGB (Digital)  
Sensor de lluvia  (Digital)   
LDR (Fotorresistencia) (Analogico)   
Led LDR (Digital con comportamiento analogico)    
Led humedad (Digital con comportamiento analogico)    
