#pragma pack(push)
#pragma pack(1)

typedef struct {
    char JFIF[5];
    uint8_t version[2];
    uint8_t density_unit;
    uint16_t density_x;
    uint16_t density_y;
    uint8_t thumb_w;
    uint8_t thumb_h;
    uint8_t thumb_data[0];
} APP0Segment;

typedef struct {
} DQTSegment;

typedef struct {
} DHTSegment;

typedef struct {
} SOSSegment;

#pragma pack(pop)

typedef enum {
    SOI,
    EOI,
    APP0,
    DQT,
    SOF0,
    DHT,
    SOS,
    UNKNOWN
} MarkerType;

typedef struct {
    MarkerType type;
    uint16_t len;
    union {
        APP0Segment *APP0;
        DQTSegment *DQT;
        DHTSegment *DHT;
        SOSSegment *SOS;
        void *ptr;
    } data;
} MarkerSegment;

MarkerType marker_type(uint16_t marker);
MarkerSegment *read_segment(FILE *fp);
