#!/usr/bin/env ruby

require 'rubygems'
require 'yaml'
require 'serialport'


config = YAML.load_file(File.join(File.dirname(__FILE__), '/hackprint.yml'))
@config = config[config["station"]]

begin
  @serial = SerialPort.new( @config["port"], "baud" => @config["baud"], "data_bits" => 8 )
rescue Exception => ex
  STDERR.puts "Could not open serial port (#{@config['port']}). Check settings."
  STDERR.puts ex
  Process.exit
end

@open_time = @config["time"].chr
@password = @config["pass"]

loop do
  File.open(@config["authpipe"], 'r') do |f|
    line = f.readline.chomp
    if line == @password
      begin
        @serial.write(@open_time)
        puts "Unlock"
      rescue Exception => ex
        STDERR.puts "Caught error trying to unlock door: #{ex.inspect}"
      end
    end
  end
end

