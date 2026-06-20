package com.lifeteam.farbeat.demo.util;

import com.lifeteam.farbeat.demo.app.NingApp;
import com.lifeteam.farbeat.demo.bean.InfoBean;
import com.lifeteam.farbeat.demo.bean.InterExcel;
import com.lifeteam.farbeat.util.time.TimeFormat;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import jxl.Workbook;
import jxl.WorkbookSettings;
import jxl.format.Alignment;
import jxl.format.Border;
import jxl.format.BorderLineStyle;
import jxl.format.Colour;
import jxl.format.VerticalAlignment;
import jxl.read.biff.BiffException;
import jxl.write.Label;
import jxl.write.WritableCellFormat;
import jxl.write.WritableFont;
import jxl.write.WritableSheet;
import jxl.write.WritableWorkbook;
import jxl.write.WriteException;

public class ExcelUtil {
    private static WritableCellFormat arial14format = null;//单元格格式
    private static WritableCellFormat arial10format = null;
    private static WritableCellFormat arial10formatNoBold = null;
    private static WritableCellFormat arial12format = null;
    private static WritableCellFormat arial12formatLeft = null;
    private final static String UTF8_ENCODING = "UTF-8";

    //单元格的格式设置 字体大小 颜色 对齐方式、背景颜色等...
    private static void format() {
        try {
            //字体 ARIAL， 字号 14  bold  粗体
            WritableFont arial14font = new WritableFont(WritableFont.ARIAL, 14, WritableFont.BOLD);
            arial14font.setColour(Colour.BLACK);//字体的颜色
//            arial14font.setUnderlineStyle(UnderlineStyle.SINGLE);//设置下划线

            //初始化单元格格式
            // 表头
            arial14format = new WritableCellFormat(arial14font);
            arial14format.setAlignment(Alignment.CENTRE);//对齐方式
            arial14format.setBorder(Border.ALL, BorderLineStyle.THIN);//边框的格式
            arial14format.setBackground(Colour.GRAY_25);//底色
            arial14format.setVerticalAlignment(VerticalAlignment.CENTRE);

            // 标题
            WritableFont arial10font = new WritableFont(WritableFont.ARIAL, 10, WritableFont.BOLD);
            arial10format = new WritableCellFormat(arial10font);
            arial10format.setAlignment(Alignment.CENTRE);
            arial10format.setBorder(Border.ALL, BorderLineStyle.THIN);
            arial10format.setBackground(Colour.GRAY_25);
            arial10format.setVerticalAlignment(VerticalAlignment.CENTRE);

            // 标题不加粗
            WritableFont arial10fontNoBold = new WritableFont(WritableFont.ARIAL, 10, WritableFont.NO_BOLD);
            arial10formatNoBold = new WritableCellFormat(arial10fontNoBold);
            arial10formatNoBold.setAlignment(Alignment.CENTRE);
            arial10formatNoBold.setBorder(Border.ALL, BorderLineStyle.THIN);
            arial10formatNoBold.setBackground(Colour.GRAY_25);
            arial10formatNoBold.setVerticalAlignment(VerticalAlignment.CENTRE);

            // 内容
            WritableFont arial12font = new WritableFont(WritableFont.ARIAL, 10);
            arial12format = new WritableCellFormat(arial12font);
            arial12format.setAlignment(Alignment.CENTRE);
            arial12format.setBorder(Border.ALL, BorderLineStyle.THIN);
            arial12format.setVerticalAlignment(VerticalAlignment.CENTRE);

            // 内容靠左
            arial12formatLeft = new WritableCellFormat(arial12font);
            arial12formatLeft.setAlignment(Alignment.LEFT);
            arial12formatLeft.setBorder(Border.ALL, BorderLineStyle.THIN);
            arial12formatLeft.setVerticalAlignment(VerticalAlignment.CENTRE);
        } catch (WriteException e) {
            e.printStackTrace();
        }
    }

    /**
     * 初始化Excel
     * 写入字段名称，表名
     *
     * @param filePath  导出excel的存放地址
     * @param sheetName Excel表格的表名
     * @param colName   excel中包含的列名
     */
    public static void initExcel(String filePath, String sheetName, String[] colName, InfoBean info) {
        format();
        //创建一个工作薄，就是整个Excel文档
        WritableWorkbook workbook = null;
        try {
            File file = new File(filePath);
            if (!file.exists()) {
                boolean newFile = file.createNewFile();
            }
            //使用Workbook创建一个工作薄，就是整个Excel文档
            workbook = Workbook.createWorkbook(file);
            //设置表格的名称(两个参数分别是工作表名字和插入位置，这个位置从0开始)
            WritableSheet sheet = workbook.createSheet(sheetName, 0);
            //创建label标签：实际就是单元格的标签（三个参数分别是：col + 1列，row + 1行， 内容， 单元格格式）
//            Label label = new Label(0, 0, filePath, arial14format);//设置第一行的单元格标签为：标题
            //将标签加入到工作表中
//            sheet.addCell(label);

            sheet.setColumnView(0, 20);
            sheet.setColumnView(1, 40);
            sheet.setColumnView(2, 20);
            sheet.setColumnView(3, 40);

            //通过writablesheet.mergeCells(int x,int y,int m,int n);来实现的。
            // 表示将从第x+1列，y+1行到m+1列，n+1行合并 (四个点定义了两个坐标，左上角和右下角)
            sheet.mergeCells(0, 0, colName.length - 1, 0);
            sheet.addCell(new Label(0, 0, sheetName, arial14format));
            sheet.setRowView(0, 520);

            sheet.addCell(new Label(0, 1, "检测终端", arial10formatNoBold));
            sheet.addCell(new Label(2, 1, "应用版本", arial10formatNoBold));
            sheet.addCell(new Label(0, 2, "健康设备", arial10formatNoBold));
            sheet.addCell(new Label(2, 2, "设备MAC", arial10formatNoBold));
            sheet.addCell(new Label(0, 3, "测试人", arial10formatNoBold));
            sheet.addCell(new Label(2, 3, "送检人", arial10formatNoBold));
            sheet.addCell(new Label(0, 4, "测试时间", arial10formatNoBold));
            sheet.addCell(new Label(2, 4, "测试公司", arial10formatNoBold));

            if (info != null) {
                sheet.addCell(new Label(1, 1, NingApp.phoneInfo, arial12format));
                sheet.addCell(new Label(3, 1, "v" + NingApp.versionName, arial12format));
                sheet.addCell(new Label(1, 2, NingApp.currentDeviceName, arial12format));
                sheet.addCell(new Label(3, 2, NingApp.currentDeviceMac, arial12format));
                sheet.addCell(new Label(1, 3, info.getTester(), arial12format));
                sheet.addCell(new Label(3, 3, info.getSubmitter(), arial12format));
                String systemDate = TimeFormat.getSystemDate(TimeFormat.FEN_DATE);
                sheet.addCell(new Label(1, 4, systemDate, arial12format));
                sheet.addCell(new Label(3, 4, info.getAddress(), arial12format));
            }

            sheet.mergeCells(0, 5, colName.length - 1, 5);
            sheet.addCell(new Label(0, 5, "测试内容", arial10format));

            sheet.mergeCells(1, 6, 2, 6);
            //再同一个单元格中写入数据，上一个数据会被下一个数据覆盖
            for (int col = 0; col < colName.length; col++) {
                sheet.addCell(new Label(col, 6, colName[col], arial10format));
            }
            //设置行高 参数的意义为（第几行， 行高）
            for (int i = 1; i <= 6; i++) {
                sheet.setRowView(i, 350);
            }
            workbook.write();// 写入数据
        } catch (IOException | WriteException e) {
            e.printStackTrace();
        } finally {
            if (workbook != null) {
                try {
                    workbook.close();// 关闭文件
                } catch (IOException | WriteException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    /**
     * 见指定类型的List写入到Excel文件中
     *
     * @param objList  代写入的List
     * @param fileName
     * @param <T>
     */
    public static <T> void writeObjListToExcel(List<T> objList, String fileName, String testConclusion) {
        if (objList != null && objList.size() > 0) {
            //创建一个工作薄，就是整个Excel文档
            WritableWorkbook writeBook = null;
            InputStream in = null;
            try {
                WorkbookSettings settings = new WorkbookSettings();
                settings.setEncoding(UTF8_ENCODING);
                in = new FileInputStream(new File(fileName));
                //Workbook不但能用来创建工作薄，也可以读取现有的工作薄
                Workbook workbook = Workbook.getWorkbook(in);
                //创建一个工作薄，就是整个Excel文档
                writeBook = Workbook.createWorkbook(new File(fileName), workbook);
                //读取表格
                WritableSheet sheet = writeBook.getSheet(0);

                for (int j = 0; j < objList.size(); j++) {

                    InterExcel interExcel = (InterExcel) objList.get(j);
                    List<String> list = new ArrayList<>();
                    list.add(String.valueOf(interExcel.getNumber()));
                    list.add(interExcel.getTitle());
                    list.add(interExcel.getResult());

                    sheet.mergeCells(1, j + 7, 2, j + 7);

                    for (int i = 0; i < list.size(); i++) {
                        String s = list.get(i);
                        switch (i) {
                            case 0:
                                sheet.addCell(new Label(i, j + 7, s, arial12format));//向一行中添加数据
                                break;
                            case 1:
                                sheet.addCell(new Label(i, j + 7, s, arial12formatLeft));//向一行中添加数据
                                break;
                            case 2:
                                sheet.addCell(new Label(i + 1, j + 7, s, arial12format));//向一行中添加数据
                                break;
                        }

//                        if (s.length() <= 4) {
//                            //设置列宽
//                            sheet.setColumnView(i, s.length() + 8);
//                        } else {
//                            sheet.setColumnView(i, s.length() + 30);
//                        }
                    }
                    //设置行高
                    sheet.setRowView(j + 7, 350);
                }

                sheet.addCell(new Label(0, objList.size() + 7, "检测结论", arial10format));
                sheet.mergeCells(1, objList.size() + 7, 3, objList.size() + 7);
                sheet.addCell(new Label(1, objList.size() + 7, testConclusion, arial12formatLeft));
                sheet.setRowView(objList.size() + 7, 700);

                writeBook.write();
                workbook.close();
                // 导出Excel成功
            } catch (IOException | BiffException | WriteException e) {
                e.printStackTrace();
            } finally {
                if (writeBook != null) {
                    try {
                        writeBook.close();
                    } catch (IOException | WriteException e) {
                        e.printStackTrace();
                    }
                }
                if (in != null) {
                    try {
                        in.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
    }
}