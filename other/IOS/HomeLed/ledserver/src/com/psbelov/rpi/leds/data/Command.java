package com.psbelov.rpi.leds.data;

import java.io.Serializable;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;

public class Command implements Serializable {
    private CommandsEnum command;
    private String data;
    private double doubleParam;
    private boolean booleanParam;
    private JSONArray list;
    
    public Command(CommandsEnum command, String data) {
        this.command=command;
        this.data=data;
    }
    public Command(CommandsEnum command, double longParam, boolean booleanParam) {
        this.command=command;
        this.doubleParam=longParam;
        this.booleanParam=booleanParam;
    }
    public Command(CommandsEnum command, JSONArray list) {
        this.command=command;
        this.list=list;
    }
    
    public CommandsEnum getCommand() {
        return command;
    }
    
    public Object getData() {
        return data;
    }
    public JSONArray getArray() {
        return list;
    }

    public Integer getIntData() {
        Integer data = null;

        if (this.data == null) {
            return null;
        }

        try {
            data = Integer.parseInt(this.data);
        } catch (NumberFormatException nfe) {
            nfe.printStackTrace();

        }

        return data;
    }

    public String getStringData() {
        return data;
    }
}
