#include "mss-signals.h"

void SignalHead::setAspect(SignalAspect_t aspect)
{
	this->aspect = aspect;
}

SignalAspect_t SignalHead::getAspect()
{
	return this->aspect;
}

const char* SignalHead::getAspectText()
{
	SignalAspect_t aspect = this->getAspect();

	switch(aspect)
	{
		case ASPECT_OFF:
			return "OFF";
		case ASPECT_GREEN:
			return "GRN";
		case ASPECT_FL_GREEN:
			return "F-GRN";
		case ASPECT_YELLOW:
			return "YLW";
		case ASPECT_FL_YELLOW:
			return "F-YLW";
		case ASPECT_RED:
			return "RED";
		case ASPECT_FL_RED:
			return "F-RED";
		case ASPECT_LUNAR:
			return "LNR";
		default:
			return "UNK";
	}
}


SignalHead::SignalHead() 
{
	this->aspect = ASPECT_OFF;
	this->headType = SIGNAL_HEAD_THREE_LIGHT;
	this->headPolarity = SIGNAL_POL_BOARD_SENSING;
	this->calculateConfigByte();
}

void SignalHead::setSignalHeadType(SignalHeadType_t headType)
{
	if (headType >= SIGNAL_HEAD_END)
		return;
	if (this->headType != headType)
	{
		this->headType = headType;
		this->calculateConfigByte();
	}
}

void SignalHead::setSignalHeadPolarity(SignalPolarity_t headPolarity)
{
	if (headPolarity >= SIGNAL_POL_END)
		return;
	if (this->headPolarity != headPolarity)
	{
		this->headPolarity = headPolarity;
		this->calculateConfigByte();
	}
}

uint8_t SignalHead::getConfigByte()
{
	return this->config;
}

void SignalHead::calculateConfigByte()
{
	uint8_t configByte = 0;
	switch (this->headType)
	{
		case SIGNAL_HEAD_THREE_LIGHT:
			configByte |= SignalHead::OPTION_SIGNAL_THREE_LIGHT;
			break;
		case OPTION_SIGNAL_SEARCHLIGHT:
			configByte |= SignalHead::OPTION_SIGNAL_SEARCHLIGHT;
			break;
		default:
			break;
	}

	switch (this->headPolarity)
	{
		case SIGNAL_POL_BOARD_SENSING:
			configByte |= SignalHead::OPTION_CA_CC_SENSE;
			break;
		case SIGNAL_POL_COMMON_ANODE:
			configByte |= SignalHead::OPTION_COMMON_ANODE;
			break;
		case SIGNAL_POL_COMMON_CATHODE:
			configByte |= SignalHead::OPTION_COMMON_CATHODE;
			break;
		default:
			break;
	}
	this->config = configByte;
}

SignalController::SignalController()
{
	this->wireMux = NULL;
	this->muxID = 255;
	memset(this->lastConfigBuffer, 255, sizeof(SignalController::lastConfigBuffer));
}

void SignalController::begin(WireMux* wireMux, uint8_t muxID)
{
	this->wireMux = wireMux;
	this->muxID = muxID;
	this->A1.setAspect(ASPECT_OFF);
	this->A2.setAspect(ASPECT_OFF);
	this->B1.setAspect(ASPECT_OFF);
	this->B2.setAspect(ASPECT_OFF);
	this->C1.setAspect(ASPECT_OFF);
	this->C2.setAspect(ASPECT_OFF);
	this->D1.setAspect(ASPECT_OFF);
	this->D2.setAspect(ASPECT_OFF);
	this->updateOutputs();
}

void SignalController::updateConfig()
{
	uint8_t configBuffer[8];
	configBuffer[0] = this->A1.getConfigByte();
	configBuffer[1] = this->A2.getConfigByte();
	configBuffer[2] = this->B1.getConfigByte();
	configBuffer[3] = this->B2.getConfigByte();
	configBuffer[4] = this->C1.getConfigByte();
	configBuffer[5] = this->C2.getConfigByte();
	configBuffer[6] = this->D1.getConfigByte();
	configBuffer[7] = this->D2.getConfigByte();

	if (0 == memcmp(configBuffer, this->lastConfigBuffer, sizeof(SignalController::lastConfigBuffer)))
		return; // Done, nothing to do here
	
	this->wireMux->setPort(this->muxID);
	this->wireMux->getWire()->beginTransmission(SignalController::SIGNAL_CONTROLLER_I2C_ADDR);
	this->wireMux->getWire()->write(0x08);  // Start of options
	for(uint8_t i=0; i<8; i++)
		this->wireMux->getWire()->write(configBuffer[i]);
	if (0 == this->wireMux->getWire()->endTransmission())
	{
		memcpy(this->lastConfigBuffer, configBuffer, sizeof(SignalController::lastConfigBuffer));
	}
}

void SignalController::updateOutputs()
{
	this->updateConfig(); // Update config if needed

	this->wireMux->setPort(this->muxID);
	this->wireMux->getWire()->beginTransmission(SignalController::SIGNAL_CONTROLLER_I2C_ADDR);
	this->wireMux->getWire()->write(0x00);  // Start of aspects
	this->wireMux->getWire()->write(this->A1.getAspect());
	this->wireMux->getWire()->write(this->A2.getAspect());
	this->wireMux->getWire()->write(this->B1.getAspect());
	this->wireMux->getWire()->write(this->B2.getAspect());
	this->wireMux->getWire()->write(this->C1.getAspect());
	this->wireMux->getWire()->write(this->C2.getAspect());
	this->wireMux->getWire()->write(this->D1.getAspect());
	this->wireMux->getWire()->write(this->D2.getAspect());
	this->wireMux->getWire()->endTransmission();
}
