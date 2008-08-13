#!/usr/bin/env ruby

require 'benchmark_setup'

File.open("#{File.basename(__FILE__, '.rb')}.dat", 'w') do |f|
  user = @users.first
  index = 0
  
  blacklist = @server.blacklist
  
  bm = Benchmark.measure do
    50.times do
      user.add_blacklist(blacklist)
    end
  end
  
  bm /= 50
  
  f << "#{index}\t#{bm.real}\n"
  
  @users.each_with_index do |user, index|
    tickets = [ user.ticket(@server.server_id) ]
    @server.blacklist = @nm.update_blacklist(@server.server_id, @server.blacklist, tickets)
    
    blacklist = @server.blacklist
    
    bm = Benchmark.measure do
      50.times do
        user.add_blacklist(blacklist)
      end
    end
    
    bm /= 50
    
    f << "#{index+1}\t#{bm.real}\n"
    
    puts "#{Time.now}\t#{index}" if index % 100 == 0
  end
end
