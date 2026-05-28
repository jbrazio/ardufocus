# Declare module of your plugin under Jekyll module
module Jekyll::CustomFilterURL
  def absolute_url(input)
    # Jekyll environment variable
    env  = ENV['JEKYLL_ENV'] || 'development'
    url  = Jekyll.configuration({})['url'] || ''
    base = Jekyll.configuration({})['doks']['baseurl'] || ''

    # Check if environment is development
    is_development = env == 'development'

    # Set variable for
    url_prepend  = is_development ? '' : url
    base_prepend = is_development ? '' : base

    # Return result URL string
    "#{url_prepend}#{base_prepend}#{input}"
  end

  def relative_url(input)
    # Jekyll environment variable
    env  = ENV['JEKYLL_ENV'] || 'development'
    base = Jekyll.configuration({})['doks']['baseurl'] || ''

    # Check if environment is development
    is_development = env == 'development'

    # Set variable for
    base_prepend = is_development ? '' : base

    # Return result URL string
    "#{base_prepend}#{input}"
  end
end

Liquid::Template.register_filter(Jekyll::CustomFilterURL)
