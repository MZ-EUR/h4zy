helpers do

  def req_info(request)
    ua = request.env["HTTP_USER_AGENT"]
    remote_addr = request.env["HTTP_X_FORWARDED_FOR"]
    remote_addr ||= request.env["REMOTE_ADDR"]

    return ua, remote_addr
  end

  def host_create(affid, remote_addr, ua)

    host = Host.new

    # it's ok to have a host NOT associated with an affiliate
    if affid.nil? then
      host.unknown = true
    else
      host.affiliate_id = affid
    end

    host.ip_addr = remote_addr

    host.browser, host.os_vers = parseUA(ua)

    ginfo = Geokit::Geocoders::MultiGeocoder.geocode(host.ip_addr)
    host.city = ginfo.city
    host.state = ginfo.state
    host.country = ginfo.country_code

    host.save

    return host
  end

  def dollars_to_cents(amount)
    return amount.to_f * 100
  end

  def cents_to_dollars(amount)
    amount/100.to_f
  end

  # converts from cents to dollars and adds formatting
  def prettymoney(amount)
    '$%.2f' % cents_to_dollars(amount).to_s
  end

  def current_user
    return nil if session[:id].nil?
    return User.get(session[:id])
  end

  def protected!
    unless !session[:id].nil?
      redirect '/'
    end

    u = User.get(session[:id])

    if u.nil? then
      redirect '/'
    end

  end

  def admin!
    unless !session[:id].nil?
      redirect '/'
    end

    u = User.get(session[:id])

    if u.nil? then
      redirect '/'
    end

    unless u.admin?
      redirect '/'
    end
  end

  def parseUA(ua)
    #ua = Uaid::UserAgent.new ua
    agent = Agent.new ua

    return agent.name, agent.os
  end

  def isdomain(domain)
    flag = true

    if domain.match(/http:\/\//).nil?
      flag = false
    end

    if domain.match(/\./).nil? then
      flag = false
    end

    return flag
  end

  def resizeimg(ofilename, nfilename)
    img = Magick::Image.read(ofilename).first
    img.crop_resized!(110, 85, Magick::NorthGravity)
    img.write('/tmp/' + nfilename)
  end

  def plaintext(filename)
    File.open(filename) do |f|
      @blah = f.read
    end
  end

  def stores3(filename, filepath)
    AWS::S3::Base.establish_connection!(
      :access_key_id     => 'mr. garrison',
      :secret_access_key => 'suck my balls'
    )
    AWS::S3::S3Object.store(filename, open(filepath), 'social_imgs', :access => :public_read)
    AWS::S3::Base.disconnect!()
  end

end
