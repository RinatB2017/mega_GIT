package com.kuldiegor.recognize;

/**
 * Created by aeterneus on 07.01.2017.
 */
public class StringTool {
    public static String parse(String text,String s1,String s2,IntBox intBox){
        int first = text.indexOf(s1,intBox.offset);
        if (first==-1){
            return "";
        }
        first +=s1.length();
        int last = text.indexOf(s2,first);
        if (s2.isEmpty()){
            last=text.length();
        }
        if (last==-1){
            return "";
        }
        intBox.offset = last;
        return text.substring(first,last);
    }
    public static String parse(String text,String s1,String s2){
        IntBox intBox = new IntBox(0);
        return parse(text,s1,s2,intBox);

    }
}
