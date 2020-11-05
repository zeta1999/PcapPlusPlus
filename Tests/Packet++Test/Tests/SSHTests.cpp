#include "../TestDefinition.h"
#include "../Utils/TestUtils.h"
#include "Packet.h"
#include "SSHLayer.h"

PTF_TEST_CASE(SSHParsingTest)
{
	timeval time;
	gettimeofday(&time, NULL);

	READ_FILE_AND_CREATE_PACKET(1, "PacketExamples/SSHIdentification.dat");
	pcpp::Packet sshIdentificationPacket(&rawPacket1);

	// SSH Identification
	PTF_ASSERT_TRUE(sshIdentificationPacket.isPacketOfType(pcpp::SSH));
	pcpp::SSHIdentificationMessage* sshIdentLayer = sshIdentificationPacket.getLayerOfType<pcpp::SSHIdentificationMessage>();
	PTF_ASSERT_NOT_NULL(sshIdentLayer);
	PTF_ASSERT_EQUAL(sshIdentLayer->getIdentificationMessage(), "SSH-2.0-OpenSSH_5.3", string);


	// SSH Key Exchange Init
	READ_FILE_AND_CREATE_PACKET(2, "PacketExamples/SSHKexInit.dat");
	pcpp::Packet sshKexInitPacket(&rawPacket2);
	PTF_ASSERT_TRUE(sshKexInitPacket.isPacketOfType(pcpp::SSH));
	pcpp::SSHKeyExchangeInitMessage* sshKexInitLayer = sshKexInitPacket.getLayerOfType<pcpp::SSHKeyExchangeInitMessage>();
	PTF_ASSERT_NOT_NULL(sshKexInitLayer);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getHeaderLen(), 904, size);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getPaddingLength(), 4, size);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getSSHHandshakeMessageLength(), 894, size);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getMessageType(), pcpp::SSHHandshakeMessage::SSH_MSG_KEX_INIT, enum);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getMessageTypeStr(), "Key Exchange Init", string);
	PTF_ASSERT_EQUAL(sshKexInitLayer->toString(), "SSH Layer, Handshake Message: Key Exchange Init", string);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getCookieAsHexStream(), "0e7b9a0807ea71e0945dd1fdbfa41929", string);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getKeyExchangeAlgorithms(), "diffie-hellman-group-exchange-sha256,diffie-hellman-group-exchange-sha1,diffie-hellman-group14-sha1,diffie-hellman-group1-sha1", string);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getServerHostKeyAlgorithms(), "ssh-rsa-cert-v01@openssh.com,ssh-dss-cert-v01@openssh.com,ssh-rsa-cert-v00@openssh.com,ssh-dss-cert-v00@openssh.com,ssh-rsa,ssh-dss", string);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getEncryptionAlgorithmsClientToServer(), "aes128-ctr,aes192-ctr,aes256-ctr,arcfour256,arcfour128,aes128-cbc,3des-cbc,blowfish-cbc,cast128-cbc,aes192-cbc,aes256-cbc,arcfour,rijndael-cbc@lysator.liu.se", string);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getEncryptionAlgorithmsServerToClient(), "aes128-ctr,aes192-ctr,aes256-ctr,arcfour256,arcfour128,aes128-cbc,3des-cbc,blowfish-cbc,cast128-cbc,aes192-cbc,aes256-cbc,arcfour,rijndael-cbc@lysator.liu.se", string);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getMacAlgorithmsClientToServer(), "hmac-md5,hmac-sha1,umac-64@openssh.com,hmac-ripemd160,hmac-ripemd160@openssh.com,hmac-sha1-96,hmac-md5-96", string);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getMacAlgorithmsServerToClient(), "hmac-md5,hmac-sha1,umac-64@openssh.com,hmac-ripemd160,hmac-ripemd160@openssh.com,hmac-sha1-96,hmac-md5-96", string);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getCompressionAlgorithmsClientToServer(), "none,zlib@openssh.com,zlib", string);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getCompressionAlgorithmsServerToClient(), "none,zlib@openssh.com,zlib", string);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getLanguagesClientToServer(), "", string);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getLanguagesServerToClient(), "", string);
	PTF_ASSERT_FALSE(sshKexInitLayer->isFirstKexPacketFollows());


	READ_FILE_AND_CREATE_PACKET(3, "PacketExamples/SSHNewKeysAndKexDH.dat");
	pcpp::Packet sshNewKeysKexDHPacket(&rawPacket3);
	PTF_ASSERT_TRUE(sshNewKeysKexDHPacket.isPacketOfType(pcpp::SSH));
	// SSH DH Group Exchange Reply
	pcpp::SSHHandshakeMessage* sshHandshakeMessage = sshNewKeysKexDHPacket.getLayerOfType<pcpp::SSHHandshakeMessage>();
	PTF_ASSERT_NOT_NULL(sshHandshakeMessage);
	PTF_ASSERT_EQUAL(sshHandshakeMessage->getHeaderLen(), 704, size);
	PTF_ASSERT_EQUAL(sshHandshakeMessage->getPaddingLength(), 7, size);
	PTF_ASSERT_EQUAL(sshHandshakeMessage->getSSHHandshakeMessageLength(), 691, size);
	PTF_ASSERT_EQUAL(sshHandshakeMessage->getMessageType(), pcpp::SSHHandshakeMessage::SSH_MSG_KEX_DH_GEX_REPLY, enum);
	PTF_ASSERT_EQUAL(sshHandshakeMessage->getMessageTypeStr(), "Diffie-Hellman Group Exchange Reply", string);
	PTF_ASSERT_EQUAL(sshHandshakeMessage->toString(), "SSH Layer, Handshake Message: Diffie-Hellman Group Exchange Reply", string);

	// SSH New Keys
	sshHandshakeMessage = sshNewKeysKexDHPacket.getNextLayerOfType<pcpp::SSHHandshakeMessage>(sshHandshakeMessage);
	PTF_ASSERT_NOT_NULL(sshHandshakeMessage);
	PTF_ASSERT_EQUAL(sshHandshakeMessage->getHeaderLen(), 16, size);
	PTF_ASSERT_EQUAL(sshHandshakeMessage->getPaddingLength(), 10, size);
	PTF_ASSERT_EQUAL(sshHandshakeMessage->getSSHHandshakeMessageLength(), 0, size);
	PTF_ASSERT_EQUAL(sshHandshakeMessage->getMessageType(), pcpp::SSHHandshakeMessage::SSH_MSG_NEW_KEYS, enum);
	PTF_ASSERT_EQUAL(sshHandshakeMessage->getMessageTypeStr(), "New Keys", string);
	PTF_ASSERT_EQUAL(sshHandshakeMessage->toString(), "SSH Layer, Handshake Message: New Keys", string);


	// SSH DH Group Exchange Init
	READ_FILE_AND_CREATE_PACKET(4, "PacketExamples/SSHDHGexInit.dat");
	pcpp::Packet sshDHGexInit(&rawPacket4);
	PTF_ASSERT_TRUE(sshDHGexInit.isPacketOfType(pcpp::SSH));
	sshHandshakeMessage = sshDHGexInit.getLayerOfType<pcpp::SSHHandshakeMessage>();
	PTF_ASSERT_NOT_NULL(sshHandshakeMessage);
	PTF_ASSERT_EQUAL(sshHandshakeMessage->getHeaderLen(), 144, size);
	PTF_ASSERT_EQUAL(sshHandshakeMessage->getPaddingLength(), 5, size);
	PTF_ASSERT_EQUAL(sshHandshakeMessage->getSSHHandshakeMessageLength(), 133, size);
	PTF_ASSERT_EQUAL(sshHandshakeMessage->getMessageType(), pcpp::SSHHandshakeMessage::SSH_MSG_KEX_DH_GEX_INIT, enum);
	PTF_ASSERT_EQUAL(sshHandshakeMessage->getMessageTypeStr(), "Diffie-Hellman Group Exchange Init", string);
	PTF_ASSERT_EQUAL(sshHandshakeMessage->toString(), "SSH Layer, Handshake Message: Diffie-Hellman Group Exchange Init", string);


	// SSH Encrypted Message
	READ_FILE_AND_CREATE_PACKET(5, "PacketExamples/SSHEncryptedMessage.dat");
	pcpp::Packet sshEncryptedPacket(&rawPacket5);
	PTF_ASSERT_TRUE(sshEncryptedPacket.isPacketOfType(pcpp::SSH));
	pcpp::SSHEncryptedMessage* sshEncryptedMsgLayer = sshEncryptedPacket.getLayerOfType<pcpp::SSHEncryptedMessage>();
	PTF_ASSERT_NOT_NULL(sshEncryptedMsgLayer);
	PTF_ASSERT_EQUAL(sshEncryptedMsgLayer->getHeaderLen(), 368, size);
	PTF_ASSERT_EQUAL(sshEncryptedMsgLayer->toString(), "SSH Layer, Encrypted Message", string);
}


PTF_TEST_CASE(SSHMalformedParsingTest)
{
	timeval time;
	gettimeofday(&time, NULL);

	// SSH identification message that ends with "\r" instead of "\n" or "\r\n"
	READ_FILE_AND_CREATE_PACKET(1, "PacketExamples/SSHIdentification_Malformed.dat");
	pcpp::Packet sshIdentificationMalformedPacket(&rawPacket1);
	PTF_ASSERT_TRUE(sshIdentificationMalformedPacket.isPacketOfType(pcpp::SSH));
	PTF_ASSERT_NULL(sshIdentificationMalformedPacket.getLayerOfType<pcpp::SSHKeyExchangeInitMessage>());
	PTF_ASSERT_NOT_NULL(sshIdentificationMalformedPacket.getLayerOfType<pcpp::SSHEncryptedMessage>());

	// SSH message which its packet len is larger than the whole layer len
	READ_FILE_AND_CREATE_PACKET(2, "PacketExamples/SSHMessage_MalformedLen.dat");
	pcpp::Packet sshMessageMalformedLenPacket(&rawPacket2);
	PTF_ASSERT_TRUE(sshMessageMalformedLenPacket.isPacketOfType(pcpp::SSH));
	PTF_ASSERT_NULL(sshMessageMalformedLenPacket.getLayerOfType<pcpp::SSHHandshakeMessage>());
	PTF_ASSERT_NOT_NULL(sshMessageMalformedLenPacket.getLayerOfType<pcpp::SSHEncryptedMessage>());

	// SSH message which its padding len is larger than message len
	READ_FILE_AND_CREATE_PACKET(3, "PacketExamples/SSHMessage_MalformedPaddingLen.dat");
	pcpp::Packet sshMessageMalformedPadLenPacket(&rawPacket3);
	PTF_ASSERT_TRUE(sshMessageMalformedPadLenPacket.isPacketOfType(pcpp::SSH));
	PTF_ASSERT_NULL(sshMessageMalformedPadLenPacket.getLayerOfType<pcpp::SSHHandshakeMessage>());
	PTF_ASSERT_NOT_NULL(sshMessageMalformedPadLenPacket.getLayerOfType<pcpp::SSHEncryptedMessage>());

	// SSH message with unknown message type
	READ_FILE_AND_CREATE_PACKET(4, "PacketExamples/SSHMessage_MalformedType.dat");
	pcpp::Packet sshMessageMalformedTypePacket(&rawPacket3);
	PTF_ASSERT_TRUE(sshMessageMalformedTypePacket.isPacketOfType(pcpp::SSH));
	PTF_ASSERT_NULL(sshMessageMalformedTypePacket.getLayerOfType<pcpp::SSHHandshakeMessage>());
	PTF_ASSERT_NOT_NULL(sshMessageMalformedTypePacket.getLayerOfType<pcpp::SSHEncryptedMessage>());

	//KexInit with malformed size of MacAlgorithmsClientToServer (size is greater than the size of the rest of the layer)
	READ_FILE_AND_CREATE_PACKET(5, "PacketExamples/SSHKexInit_Malformed.dat");
	pcpp::Packet sshKexInitMalformedPacket(&rawPacket5);
	PTF_ASSERT_TRUE(sshKexInitMalformedPacket.isPacketOfType(pcpp::SSH));
	pcpp::SSHKeyExchangeInitMessage* sshKexInitLayer = sshKexInitMalformedPacket.getLayerOfType<pcpp::SSHKeyExchangeInitMessage>();
	PTF_ASSERT_NOT_NULL(sshKexInitLayer);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getHeaderLen(), 904, size);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getPaddingLength(), 4, size);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getSSHHandshakeMessageLength(), 894, size);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getMessageType(), pcpp::SSHHandshakeMessage::SSH_MSG_KEX_INIT, enum);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getCookieAsHexStream(), "0e7b9a0807ea71e0945dd1fdbfa41929", string);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getKeyExchangeAlgorithms(), "diffie-hellman-group-exchange-sha256,diffie-hellman-group-exchange-sha1,diffie-hellman-group14-sha1,diffie-hellman-group1-sha1", string);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getServerHostKeyAlgorithms(), "ssh-rsa-cert-v01@openssh.com,ssh-dss-cert-v01@openssh.com,ssh-rsa-cert-v00@openssh.com,ssh-dss-cert-v00@openssh.com,ssh-rsa,ssh-dss", string);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getEncryptionAlgorithmsClientToServer(), "aes128-ctr,aes192-ctr,aes256-ctr,arcfour256,arcfour128,aes128-cbc,3des-cbc,blowfish-cbc,cast128-cbc,aes192-cbc,aes256-cbc,arcfour,rijndael-cbc@lysator.liu.se", string);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getEncryptionAlgorithmsServerToClient(), "aes128-ctr,aes192-ctr,aes256-ctr,arcfour256,arcfour128,aes128-cbc,3des-cbc,blowfish-cbc,cast128-cbc,aes192-cbc,aes256-cbc,arcfour,rijndael-cbc@lysator.liu.se", string);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getMacAlgorithmsClientToServer(), "", string);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getMacAlgorithmsServerToClient(), "", string);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getCompressionAlgorithmsClientToServer(), "", string);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getCompressionAlgorithmsServerToClient(), "", string);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getLanguagesClientToServer(), "", string);
	PTF_ASSERT_EQUAL(sshKexInitLayer->getLanguagesServerToClient(), "", string);
	PTF_ASSERT_FALSE(sshKexInitLayer->isFirstKexPacketFollows());
}