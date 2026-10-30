#include <stdio.h>
#include "student.h"

#define RISK_LOW_MAX    25.0
#define RISK_MODERATE_MAX   50.0

static double compute_average(const StudentList *list) {
    const Student *p;
    const Student *end = list->data + list->count;
    double sum = 0.0;

    for (p = list->data; p < end; p++) {
        sum += p->grade;
    }

    return sum / (double)list->count;
}

static void find_extremes(const StudentList *list, const Student **top, const Student **low) {
    const Student *p;
    const Student *end = list->data + list->count;

    *top = *low = list->data;
    for (p = list->data + 1; p < end; p++) {
        if (p->grade > (*top)->grade) *top = p;
        if (p->grade < (*low)->grade) *low = p;
    }
}

void calculate_average(StudentList *list) {
    if (list->count == 0) {
        printf("No record, so there is no average.\n");
        return;
    }
    printf("Average grade: %.2f (across %lu students)\n", compute_average(list), (unsigned long)list->count);
}

void find_top_student(StudentList *list) {
    const Student *top, *low, *p;
    const Student *end = list->data + list->count;
    
    if (list->count == 0) {
        printf("No records.\n");
        return;
    }

    find_extremes(list, &top, &low);

    printf("Top-performing students(s), grade %.1f:\n", top->grade);
    for (p = list->data; p < end; p++) {
        if (p->grade == top->grade) {
            printf(" %d | %s | %s\n", p->id, p->name, p->course);
        }
    }
}

typedef struct {
    size_t safe, low, moderate, high;
    double avg_risk;
    const Student *riskiest;
} RiskSummary;

static double risk_index(float grade) {
    double ari = ((double)SAFE_LEVEL - grade) / SAFE_LEVEL * 100.0;
    return ari > 0.0 ? ari : 0.0;
}

static int risk_category(double ari) {
    if (ari <= 0.0) {
        return 0;
    }
    if (ari <= RISK_LOW_MAX) {
        return 1;
    }
    if (ari <= RISK_MODERATE_MAX) {
        return 2;
    }
    return 3;
}

static const char *risk_level(double ari) {
    static const char *names[] = { "Safe", "Low", "Moderate", "High" };
    return names[risk_category(ari)];
}

static void compute_risk(const StudentList *list, RiskSummary *r) {
    const Student *p;
    const Student *end = list->data + list->count;
    double total = 0.0;

    r->safe = r->low = r->moderate = r->high = 0;
    r->riskiest = list->data;

    for (p = list->data; p < end; p++) {
        double ari = risk_index(p->grade);
        total += ari;

        switch (risk_category(ari)) {
            case 0: r->safe++; 
                break;
            case 1: r->low++;
                break;
            case 2: r->moderate++;
                break;
            default: r->high++;
                break;
        }

        if (p->grade < r->riskiest->grade)
            r->riskiest = p;
    }
    r->avg_risk = total / (double)list->count;
}

void custom_analysis(StudentList *list) {
    const Student *p;
    const Student *end = list->data + list->count;
    RiskSummary r;

    printf("\n=== Academic Risk Index (ARI) ===\n");
    printf("Purpose: show how far each student is below a safe grade, so\n");
    printf("         support can go to those who need it most.\n");
    printf("Formula: ARI = max(0, (%.0f - grade) / %.0f) x 100\n", SAFE_LEVEL, SAFE_LEVEL);
    printf("Levels : 0 = Safe (grade >= %.0f) | 0-25 Low | 25-50 Moderate | above 50 High\n", SAFE_LEVEL);
    printf("Reading: higher ARI = further below the safe level. 0 = no risk,\n");
    printf("         100 = grade of 0.\n\n");

    if (list->count == 0) {
        printf("No records to analyse.\n");
        return;
    }

    compute_risk(list, &r);

    printf("%-6s %-22s %6s %7s %-8s\n", "ID", "Name", "Grade", "ARI", "Level");
    printf("-----------------------------------------------------\n");
    for (p = list->data; p < end; p++) {
        double ari = risk_index(p->grade);
        if (ari > 0.0) {
            printf("%-6d %-22.22s %6.1f %7.1f %-8s\n", p->id, p->name, p->grade, ari, risk_level(ari));
        }
    }
    printf("-----------------------------------------------------\n");

    printf("Safe: %lu | Low: %lu | Moderate: %lu | High: %lu\n", (unsigned long)r.safe, (unsigned long)r.low, 
        (unsigned long)r.moderate, (unsigned long)r.high);
    printf("Average class ARI: %.2f\n", r.avg_risk);
    printf("Highest-risk student: %d %s (grade %.1f)\n", r.riskiest->id, r.riskiest->name, r.riskiest->grade);
}

void generate_summary_report(StudentList *list) {
    const Student *p, *top, *low;
    const Student *end = list->data + list->count;
    size_t pass = 0;
    RiskSummary r;

    printf("\n========= SUMMARY REPORT =========\n");
    printf("Total records: %lu\n", (unsigned long)list->count);

    if (list->count == 0) {
        printf("No data to summarise.\n");
        return;
    }

    find_extremes(list, &top, &low);
    for (p = list->data; p < end; p++) {
        if (p->grade >= PASS_THRESHOLD) {
            pass++;
        }
    }

    printf("Average grade: %.2f\n", compute_average(list));
    printf("Highest grade: %.1f (%s)\n", top->grade, top->name);
    printf("Lowest grade : %.1f (%s)\n", low->grade, low->name);
    printf("Pass threshold: %.0f\n", PASS_THRESHOLD);
    printf("Meeting threshold: %lu\n", (unsigned long)pass);
    printf("Below threshold  : %lu\n", (unsigned long)(list->count - pass));

    compute_risk(list, &r);
    printf("\n-- Custom analysis: Academic Risk Index --\n");
    printf("Safe: %lu | Low: %lu | Moderate: %lu | High: %lu\n", (unsigned long)r.safe, (unsigned long)r.low,
           (unsigned long)r.moderate, (unsigned long)r.high);
    printf("Average class ARI: %.2f\n", r.avg_risk);
    printf("Highest-risk student: %d %s (ARI %.1f)\n", r.riskiest->id, r.riskiest->name, risk_index(r.riskiest->grade));
    printf("=====================================\n");
}