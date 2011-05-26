class User
  include DataMapper::Resource

  has 1,  :affiliate
  has 1,  :advertiser

  has n,  :campaigns
  has n,  :invoices
  has n,  :credits
  has n,  :payments

  property :id,         Serial
  property :login,      String
  property :email,      String
  property :api_key,    String
  property :admin,      Boolean
  property :banned,     Boolean
  property :ip_addr,    String
  property :balance,    Integer, :default => 0
  property :password_reset_key, String
  property :activated_at, DateTime
  property :activation_code, String
  property :crypted_password, String
  property :salt, String
  property :updated_at, DateTime
  property :created_at, DateTime, :default => lambda { DateTime.now }

  def password=(password)
    encrypt_password(password)
  end

  def self.authenticate(login, password)
    @u = first(:login => login)
    if !@u.nil? then
      @u = @u
    else
      @u = nil
    end
    @u && @u.authenticated?(password) ? @u : nil
  end

  def encrypt(password, salt)
    Digest::SHA1.hexdigest("--#{salt}--#{password}--")
  end

  def authenticated?(password)
    self.crypted_password == encrypt(password, self.salt)
  end

  def encrypt_password(password)
    return if password.blank?
    self.salt = Digest::SHA1.hexdigest("--#{Time.now.to_s}--bigb00tyb1tch3s--") if salt.blank?
    self.crypted_password = encrypt(password, self.salt)
  end

end
