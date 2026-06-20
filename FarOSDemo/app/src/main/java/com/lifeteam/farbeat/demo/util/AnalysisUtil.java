package com.lifeteam.farbeat.demo.util;

public class AnalysisUtil {

//    public static void convertHealthDataReport(HealthDataReport report, List<HealthDataSourceConfigList.HealthDataSourceConfig> configList) {
//        if (report != null && configList != null && configList.size() > 0) {
//            List<Long> sourceIds = report.getArrayOfDataSourceTemplateID();
//            List<Integer> dataSetTypes = report.getArrayOfDataSetType();
//            List<Object> healthDataSet = report.getHealthDataSet();
//            if (sourceIds.size() == healthDataSet.size()) {
//                for (int i = 0; i < sourceIds.size(); i++) {
//                    long sId = sourceIds.get(i);
//                    String schemeOfDataSourceType = null;
//                    long dataSourceType = 0;
//                    int dataType = 0;
//                    int measurePeriodExist = 0;
//                    for (HealthDataSourceConfigList.HealthDataSourceConfig config : configList) {
//                        if (sId == config.getDataSourceTemplateId()) {
//                            schemeOfDataSourceType = config.getSchemeOfDataSourceType();
//                            dataSourceType = config.getDataSourceTypeList().get(0);
//                            dataType = config.getDataTypeList().get(0);
//                            measurePeriodExist = config.getMeasurePeriodExist();
//                        }
//                    }
//                    if (!DataUtil.isEmpty(schemeOfDataSourceType) && schemeOfDataSourceType.equals(ModeType.X73)) {
//                        if (dataSourceType == SourceType.RRI) {
//                            if (dataSetTypes.get(i) == DataSetType.SINGLE) {
//                                HealthDataSetSingle setSingle = (HealthDataSetSingle) healthDataSet.get(i);
//                                if (setSingle != null) {
//                                    List<Object> healthDataList = setSingle.getHealthDataList();
//                                    if (healthDataList != null) {
//                                        int size = healthDataList.size();
//                                        if (size > 0) {
//                                            for (int j = 0; j < size; j++) {
//                                                switch (measurePeriodExist) {
//                                                    case PeriodType.NO_EXIST:
//                                                        HealthData2 health = (HealthData2) healthDataList.get(j);
//                                                        if (health != null) {
//                                                            endTimestamp = health.getTimestamp();
//                                                            if (startTimestamp == 0 && currentMinute != 0) {
//                                                                startTimestamp = endTimestamp;
//                                                                startTime = TimeFormat.convertTimestamp(TimeFormat.FEN_FULL_MS, startTimestamp);
//                                                                // 开始等待倒计时
//                                                                currentMillisInFuture = currentMinute * 60 * 1000L;
//                                                                peterTimeCountRefresh = new MainActivity.PeterTimeCountRefresh(currentMillisInFuture);
//                                                                peterTimeCountRefresh.start();
//                                                            }
//                                                            Object value = health.getValue();
//                                                            switch (dataType) {
//                                                                case DataType.SHORT:
//                                                                    Number n = (Number) value;
//                                                                    rriList.add(n.intValue());
//                                                                    break;
//                                                                case DataType.INT:
//                                                                    rriList.add((int) value);
//                                                                    break;
//                                                            }
//                                                            timestampList.add(endTimestamp);
//                                                            Message msg = new Message();
//                                                            msg.what = Config.OK;
//                                                            msg.obj = value;
//                                                            handler.sendMessage(msg);
//                                                        }
//                                                        break;
//                                                    case PeriodType.IS_EXIST:
//                                                        break;
//                                                }
//                                            }
//                                        }
//                                    }
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
}