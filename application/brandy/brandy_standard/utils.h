#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

float relu(float x);
void softmax(const float *logits, float *probs, int n);
int argmax(const float *arr, int n);

#ifdef __cplusplus
}
#endif

#endif
