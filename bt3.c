
#include <stdio.h>
#include <string.h>

#define MAX_ZONES 5
#define MAX_QRS 10
#define MAX_TICKETS_PER_ORDER 4

struct TicketZone {
    int zone_id;
    char zone_name[30];
    float ticket_price;
    int total_capacity;
    int sold_tickets;
};

struct QrCheckIn {
    char qr_code[20];
    int zone_id;
    int is_used;
};

// Function prototypes
int findZoneById(const struct TicketZone zones[], int total_zones, int zone_id);
float calculateTicketPrice(float ticket_price, int is_early_bird);
int sellTickets(struct TicketZone zones[], int total_zones,
                int zone_id, int quantity, int is_early_bird,
                float *total_cost);
int checkInQr(struct QrCheckIn qrs[], int total_qrs, const char qr_code[]);
void displayZones(const struct TicketZone zones[], int total_zones);

int main(void) {
    struct TicketZone zones[MAX_ZONES] = {
        {1, "VIP", 2000000.0f, 10, 8},
        {2, "Zone A", 1200000.0f, 20, 5},
        {3, "GA", 800000.0f, 30, 10}
    };

    struct QrCheckIn qrs[MAX_QRS] = {
        {"QR1001", 1, 0},
        {"QR1002", 2, 0},
        {"QR1003", 3, 1}
    };

    int total_zones = 3;
    int total_qrs = 3;
    float total_cost = 0.0f;

    printf("=== TICKETBOX EVENT TICKETING ===\n");

    printf("\n1. DANH SACH KHU VUC BAN DAU\n");
    displayZones(zones, total_zones);

    printf("\n2. TEST BAN 2 VE VIP - EARLY BIRD\n");
    if (sellTickets(zones, total_zones, 1, 2, 1, &total_cost)) {
        printf("Thanh tien: %.0f VND\n", total_cost);
    }

    printf("\n3. TEST SO LUONG AM (-2)\n");
    sellTickets(zones, total_zones, 1, -2, 0, &total_cost);

    printf("\n4. TEST SO LUONG BANG 0\n");
    sellTickets(zones, total_zones, 1, 0, 0, &total_cost);

    printf("\n5. TEST VUOT GIOI HAN (5 VE)\n");
    sellTickets(zones, total_zones, 2, 5, 0, &total_cost);

    printf("\n6. TEST VUOT SUC CHUA VIP\n");
    sellTickets(zones, total_zones, 1, 3, 0, &total_cost);

    printf("\n7. TEST KHU VUC KHONG TON TAI\n");
    sellTickets(zones, total_zones, 99, 1, 0, &total_cost);

    printf("\n8. TEST CHECK-IN QR1001 LAN DAU\n");
    checkInQr(qrs, total_qrs, "QR1001");

    printf("\n9. TEST CHECK-IN QR1001 LAN HAI\n");
    checkInQr(qrs, total_qrs, "QR1001");

    printf("\n10. TEST QR KHONG TON TAI\n");
    checkInQr(qrs, total_qrs, "QR9999");

    printf("\n11. TEST DANH SACH RONG\n");
    displayZones(zones, 0);
    checkInQr(qrs, 0, "QR1001");

    printf("\n12. DANH SACH KHU VUC SAU KIEM THU\n");
    displayZones(zones, total_zones);

    return 0;
}

// Tim khu vuc theo ma, tra ve -1 neu khong tim thay
int findZoneById(const struct TicketZone zones[],
                 int total_zones, int zone_id) {
    if (total_zones <= 0) {
        return -1;
    }

    for (int i = 0; i < total_zones; i++) {
        if (zones[i].zone_id == zone_id) {
            return i;
        }
    }

    return -1;
}

// Tinh gia ve sau khi ap dung Early Bird
float calculateTicketPrice(float ticket_price, int is_early_bird) {
    if (is_early_bird == 1) {
        return ticket_price * 0.85f;
    }

    return ticket_price;
}

// Ban ve va cap nhat ton kho neu giao dich hop le
int sellTickets(struct TicketZone zones[], int total_zones,
                int zone_id, int quantity, int is_early_bird,
                float *total_cost) {
    if (total_zones <= 0) {
        printf("[LOI] Danh sach khu vuc dang rong.\n");
        return 0;
    }

    if (quantity < 1 || quantity > MAX_TICKETS_PER_ORDER) {
        printf("[LOI] So luong mua phai tu 1 den 4 ve.\n");
        return 0;
    }

    if (is_early_bird != 0 && is_early_bird != 1) {
        printf("[LOI] Trang thai Early Bird khong hop le.\n");
        return 0;
    }

    int index = findZoneById(zones, total_zones, zone_id);

    if (index == -1) {
        printf("[LOI] Khong tim thay khu vuc %d.\n", zone_id);
        return 0;
    }

    if (zones[index].sold_tickets + quantity >
        zones[index].total_capacity) {
        printf("[LOI] Khu vuc %s khong du ve. Giao dich bi tu choi.\n",
               zones[index].zone_name);
        return 0;
    }

    float unit_price = calculateTicketPrice(
        zones[index].ticket_price, is_early_bird);

    *total_cost = unit_price * quantity;
    zones[index].sold_tickets += quantity;

    printf("[THANH CONG] Da ban %d ve tai khu vuc %s.\n",
           quantity, zones[index].zone_name);

    return 1;
}

// Check-in QR mot lan duy nhat
int checkInQr(struct QrCheckIn qrs[], int total_qrs,
              const char qr_code[]) {
    if (total_qrs <= 0) {
        printf("[LOI] Danh sach QR dang rong.\n");
        return 0;
    }

    for (int i = 0; i < total_qrs; i++) {
        if (strcmp(qrs[i].qr_code, qr_code) == 0) {
            if (qrs[i].is_used == 1) {
                printf("[CANH BAO] QR %s da duoc su dung!\n", qr_code);
                return 0;
            }

            qrs[i].is_used = 1;
            printf("[THANH CONG] Check-in QR %s tai khu vuc %d.\n",
                   qr_code, qrs[i].zone_id);
            return 1;
        }
    }

    printf("[LOI] QR %s khong ton tai.\n", qr_code);
    return 0;
}

// Hien thi thong tin cac khu vuc
void displayZones(const struct TicketZone zones[], int total_zones) {
    if (total_zones <= 0) {
        printf("Danh sach khu vuc dang rong.\n");
        return;
    }

    for (int i = 0; i < total_zones; i++) {
        printf("Ma: %d | Khu vuc: %s | Gia goc: %.0f"
               " | Da ban: %d | Con lai: %d\n",
               zones[i].zone_id,
               zones[i].zone_name,
               zones[i].ticket_price,
               zones[i].sold_tickets,
               zones[i].total_capacity - zones[i].sold_tickets);
    }
}
