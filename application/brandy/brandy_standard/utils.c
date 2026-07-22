#include "utils.h"
#include <math.h>

float relu(float x) {
    return x > 0.0f ? x : 0.0f;
}

void softmax(const float *logits, float *probs, int n) {
    float max_logit = logits[0];
    for (int i = 1; i < n; i++) {
        if (logits[i] > max_logit) max_logit = logits[i];
    }
    float sum = 0.0f;
    for (int i = 0; i < n; i++) {
        probs[i] = expf(logits[i] - max_logit);
        sum += probs[i];
    }
    for (int i = 0; i < n; i++) {
        probs[i] /= sum;
    }
}

int argmax(const float *arr, int n) {
    int idx = 0;
    float max_val = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max_val) {
            max_val = arr[i];
            idx = i;
        }
    }
    return idx;
}
