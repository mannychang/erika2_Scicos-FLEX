using System;
using System.Collections.Generic;
using System.Text;

namespace EasylabSerialUDPGateway
{
    //Classe che si occupa di convertire i byte ricevuti nei float da visualizzare
    public class StreamParserEncoder
    {
        public const int EASYLAB_PACKET_SIZE = 2 * sizeof(float) + 2;
        public const int UDP_PACKET_SIZE = 60;

        public StreamParserEncoder()
        {
            //NewFloats = new List<float>();
            //NewPackets = new List<byte[]>();
            RemainingBytes = new byte[EASYLAB_PACKET_SIZE - 1];
        }

        public List<float> ParseBytes(byte[] newBytesRead, out byte[] prevRemaining, out byte[]  actualRemaning)
        {
            /* reset return list */
            //NewFloats.Clear();
            List<float>  NewFloats = new List<float>();

            prevRemaining = new byte[RemainingLength];
            Array.Copy(RemainingBytes, prevRemaining, RemainingLength);
            
            /* Costruisco il buffer da analizzare partendo dagli eventuali bytes rimanenti dell'invocazione precedente */
            byte[] actualPacket = new byte[RemainingLength + newBytesRead.Length];
            Array.Copy(RemainingBytes, actualPacket, RemainingLength);
            newBytesRead.CopyTo(actualPacket, RemainingLength);

            /* Parsing condition is remaining bytes are more than packet length */
            int actualIndex;
            for (actualIndex = 0; (RemainingLength = (actualPacket.Length - actualIndex)) >= EASYLAB_PACKET_SIZE; )
            {
                byte crc = 0;
                /* Start byte check.  I need 'actualIndex' incremented after this point */
                if (actualPacket[actualIndex++] != 0)
                    continue;

                /* Parsing inner loop */
                for (int j = actualIndex; j < (actualIndex + EASYLAB_PACKET_SIZE - 2); ++j)
                {
                    /* Loop to evaluate crc */
                    crc ^= actualPacket[j];
                }

                /* Check end of crc condition (-2 because 'i' is already incremented) */
                if (actualPacket[actualIndex + EASYLAB_PACKET_SIZE - 2] == crc)
                {
                    /* Valid packet read: I populate received variables */
                    NewFloats.Add(BitConverter.ToSingle(actualPacket, actualIndex));
                    NewFloats.Add(BitConverter.ToSingle(actualPacket, actualIndex + sizeof(float)));

                    /* Valid packet. I restart parsing after this one */
                    actualIndex += EASYLAB_PACKET_SIZE - 1;
                }
            }

            /* Aggiorno remaining bytes array */
            if (RemainingLength > 0)
                Array.Copy(actualPacket, actualIndex, RemainingBytes, 0, RemainingLength);

            actualRemaning = new byte[RemainingLength];
            Array.Copy(RemainingBytes, actualRemaning, RemainingLength);

            return NewFloats;
        }

        public List<byte[]> EncodeBytes(byte[] received)
        {
            /* reset return list */
            //NewPackets.Clear();
            List<byte[]> NewPackets = new List<byte[]>();


            for (int actualIndex = 0; (received.Length - actualIndex) >= UDP_PACKET_SIZE; actualIndex += UDP_PACKET_SIZE)
            {
                /* crc byte (start with actualIndex value) */
                byte crc = received[actualIndex];
                byte[] commPacket = new byte[EASYLAB_PACKET_SIZE];

                /* Crc/filling inner loop */
                for (int j = 1; j < EASYLAB_PACKET_SIZE - 1; ++j)
                {
                    /* fill evaluate packet */
                    commPacket[j] = received[actualIndex + j - 1];
                    /* evaluate crc */
                    crc ^= received[actualIndex + j];
                }

                /* add crc at the end of the packet */
                commPacket[EASYLAB_PACKET_SIZE - 1] = crc;

                /* Add new packets to the new  packets list */
                NewPackets.Add(commPacket);
            }

            return NewPackets;
        }

        private int RemainingLength;
        private readonly byte[] RemainingBytes;
        //private readonly List<float> NewFloats;
        //private readonly List<byte[]> NewPackets;
    };

}
