#ifndef __PARSERS_H
#define __PARSERS_H

void parse_APP0(APP0Segment *seg);
void parse_DQT(DQTSegment *seg, int seg_len);
void parse_SOF0(SOF0Segment *seg);
void parse_DHT(DHTSegment *seg, int seg_len);
void parse_SOS(SOSSegment *seg);
void parse_COM(MarkerSegment *seg);

#endif
