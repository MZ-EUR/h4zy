get '/' do
  if !current_user.nil? then
    redirect '/dashboard'
  else
    haml :index, :layout => :front_layout
  end
end

['/download', '/download/:affshit'].each do |path|
  get path do

    affshit = params[:affshit]

    if !affshit.nil? && !affshit.empty? then
      stuff = Base64.decode64(affshit).split(";")
    end

    dt = (Time.now + 24*60*60*365*5)

    icookie = request.cookies["identity"]

    # if not passed in just set to default affiliate
    # so we are at least tracking it
    affid ||= Affiliate.first.id

    host = Host.first(:identity => icookie)

    # make sure we only record the download once
    # although they can request the same file again
    if host.nil? then
 
      ua, remote_addr = req_info(request) 

      host = host_create(affid, remote_addr, ua)

      download = Download.new
      download.url = "/download"
      download.host_id = host.id
      download.affiliate_id = affid
      download.save

      response.set_cookie("AffiliateID", {:value => "#{affid}", :expires => dt})
      response.set_cookie("identity", {:value => "#{host.identity}", :expires => dt})
    end
    redirect '/downloads/sss.exe'

=begin
    # Browser plugins...

    if host.browser.eql? "IE" then
      redirect '/downloads/monkeyballs18.exe'
    elsif host.browser.eql? "Firefox" then
      redirect '/downloads/SuperHappyFunTime.xpi'
    elsif host.browser.eql? "Chrome" then
      redirect '/downloads/cad.crx'
    else
      redirect '/sorry.html'
    end
=end

  end

end
