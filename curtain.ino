//----------------------------------------------------------------//
// Despertador que abre e fecha a cortina nos horários definidos  //
// Autor: Rafael Faria Felix                                      //
//----------------------------------------------------------------//

//entradas
#define S1 46 //sensor 1
#define S2 47 //sensor 2
#define S3 48 //sensor 3
#define S4 49 //sensor 4

//saídas
#define M1A 50 //motor 1 - direção A
#define M1B 51 //motor 1 - direção B
#define M2A 52 //motor 2 - direção A
#define M2B 53 //motor 2 - direção B

//configuração
int WH = 6; //hora de acordar (24h)
int WM = 0; //minuto de acordar
int SH = 22; //hora de dormir (24h)
int SM = 0; //minuto de dormir

int hour=0, min=0;
int tempoAnterior = millis();

void setup() {
	Serial.begin(300);

	pinMode(S1, INPUT);
	pinMode(S2, INPUT);
	pinMode(S3, INPUT);
	pinMode(S4, INPUT);
	pinMode(M1A, OUTPUT);
	pinMode(M1B, OUTPUT);
	pinMode(M2A, OUTPUT);
	pinMode(M2B, OUTPUT);

	attachInterrupt(digitalPinToInterrupt(S1), stopM1, RAISING);
	attachInterrupt(digitalPinToInterrupt(S2), stopM1, RAISING);
	attachInterrupt(digitalPinToInterrupt(S3), stopM2, RAISING);
	attachInterrupt(digitalPinToInterrupt(S4), stopM2, RAISING);
	
	time();
}

void time() {
	if(millis() – tempoAnterior >= 60000) {
		tempoAnterior = millis();
		min += 1;
		if(min >= 60) {
			hour += 1;
			min = 0;
			if(hour >= 24) {
				hour = 0;
			}
		}
	}
	time();
}

void close() {
	digitalWrite(M1A,LOW);
	digitalWrite(M1B,HIGH);
	digitalWrite(M2A,LOW);
	digitalWrite(M2B,HIGH);
}

void open() {
	digitalWrite(M1A,HIGH);
	digitalWrite(M1B,LOW);
	digitalWrite(M2A,HIGH);
	digitalWrite(M2B,LOW);
}

void stopM1() {
	digitalWrite(M1A,LOW);
	digitalWrite(M1B,LOW);
}

void stopM2() {
	digitalWrite(M2A,LOW);
	digitalWrite(M2B,LOW);
}

void printtime() {
	if(hour < 10) {
		Serial.print("0");
	}
	Serial.print(hour);
	Serial.print(":");
	if(min < 10) {
		Serial.print("0");
	}
	Serial.println(min);
}

void printwake() {
	if(WH < 10) {
		Serial.print("0");
	}
	Serial.print(WH);
	Serial.print(":");
	if(WM < 10) {
		Serial.print("0");
	}
	Serial.println(WM);
}

void printsleep() {
	if(SH < 10) {
		Serial.print("0");
	}
	Serial.print(SH);
	Serial.print(":");
	if(SM < 10) {
		Serial.print("0");
	}
	Serial.println(SM);
}

int settime() {
	if(Serial.available() == 5) {
		int m = 0;
		char st = Serial.read();
		if(int(st) > 0) {
			m = 600 * int(st);
		}
		m += 60* int(Serial.read());
		if(Serial.read() != ':') {
			Serial.println("Favor utilizar o formato HH:MM");
			return false;
		}
		st = Serial.read();
		if(int(st) > 0) {
			m += 10 * int(st);
		}
		m += int(Serial.read());
		return m;
	} else {
		Serial.println("Favor utilizar o formato HH:MM");
		return false;
	}
}

void loop() {
	if(hour == WH && min == WM) {
		open();
	}
	
	if(hour == SH && min == SM) {
		close();
	}
	
	if (Serial.available() > 0) {
		switch(Serial.read()) {
			case 'r':
				switch(Serial.read()) {
					case 't':
						Serial.print("Hora atual: ");
						printtime();
						break;
					case 'w':
						Serial.print("Hora de despertar: ");
						printwake();
						break;
					case 's':
						Serial.print("Hora de dormir: ");
						printsleep();
						break;
					default:
						println("Para consultar as configurações use:");
						println("rt > Hora atual");
						println("rw > Hora de despertar");
						println("rs > Hora de dormir");
				}
				break;
			case 's':
				switch(Serial.read()) {
					case 't':
						int m = settime();
						if(m) {
							hour = int(m / 60);
							min = int(m - hour * 60);
							Serial.print("Hora atual definida para: ");
							printtime();
						}
						break;
					case 'w':
						int m = settime();
						if(m) {
							WH = int(m / 60);
							WM = int(m - WH * 60);
							Serial.print("Hora de despertar definida para: ");
							printwake();
						}
						break;
					case 's':
						int m = settime();
						if(m) {
							SH = int(m / 60);
							SM = int(m - SH * 60);
							Serial.print("Hora de dormir definida para: ");
							printsleep();
						}
						break;
					default:
						println("Para alterar as configurações favor utilizar os comandos a seguir:");
						println("stHH:MM > Definir a hora atual para HH:MM");
						println("swHH:MM > Definir a hora de acordar para HH:MM");
						println("ssHH:MM > Definir a hora de dormir para HH:MM");
				}
				break;
			default:
				println("Favor utilizar os comandos a seguir:");
				println("rt > Verificar a hora atual"); //read time
				println("rw > Verificar a hora de despertar"); //read wake time
				println("rs > Verificar a hora de dormir"); //read sleep time
				println("stHH:MM > Definir a hora atual para HH:MM"); //set time
				println("swHH:MM > Definir a hora de acordar para HH:MM"); //set wake time
				println("ssHH:MM > Definir a hora de dormir para HH:MM"); //set sleep time
		}
	}
}