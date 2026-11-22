#!/bin/bash

# import_films.sh: creates one WordPress post per film of films.json
# (runs on the VM)
#
# usage, from the folder holding films.json and images/:
#     sudo bash import_films.sh
#
# steps:
#   1. checks        root, tools and files are there
#   2. categories    one category per film type (Film, TV Series...)
#   3. posts         one post per film, skipped if already imported
#   4. images        each post gets its film's picture as featured image
# to add a step: add a section inside the loop (per film) or after it
#
# COMMANDS USED
#   wp             WP-CLI: manages WordPress from the terminal
#                  (posts, categories, media) instead of the browser
#   jq             reads JSON: '.[3].title' = title of the 4th film
#     -r           raw output: text without the JSON quotes
#     -c           compact: one film on one line
#     --argjson    passes a bash value into the jq program ($i)
#   runuser -u www-data --
#                  runs the next command as www-data, the user that owns
#                  the WordPress files; unlike sudo it leaves no entry
#                  in sudo.log, so monitoring.sh's count stays honest
#   mktemp -d      creates an empty temporary folder and prints its path
#
# FILES
#   films.json              list of films exported from the website
#   images/<image_id>.jpg   one picture per film (made by prepare_films.py)
#   /var/www/html           the WordPress install (wp --path)

site=/var/www/html

# STEP 1: CHECKS
# id -u prints the user number; root is 0
if [ "$(id -u)" -ne 0 ]
then
	echo "run it with: sudo bash import_films.sh"
	exit 1
fi

# command -v prints where a command is, or fails if it is not installed
for tool in wp jq
do
	if ! command -v "$tool" > /dev/null
	then
		echo "missing tool: $tool"
		exit 1
	fi
done

if [ ! -f films.json ]
then
	echo "films.json not found in $(pwd)"
	exit 1
fi

# www-data cannot enter your home folder (mode 700), so work from a
# copy it owns in /tmp
work=$(mktemp -d)
# trap ... EXIT: delete the copy when the script ends, even on an error
trap 'rm -rf "$work"' EXIT
cp films.json "$work"
if [ -d images ]
then
	cp -r images "$work"
fi
chown -R www-data:www-data "$work"

# STEP 2: CATEGORIES
# unique film types; create each one WordPress does not have yet
# grep -qx: quiet (only success/failure), whole line must match
jq -r '.[].type' "$work/films.json" | sort -u | while read -r type
do
	if ! runuser -u www-data -- wp --path="$site" term list category --field=name | grep -qx "$type"
	then
		runuser -u www-data -- wp --path="$site" term create category "$type"
	fi
done

# the post body: description, a list of details, the IMDb link
# line() adds a list item only when the value is not empty
# @html escapes characters like & and < so they show as text
page='
def line(name; value):
	if value == "" then ""
	else "<li><strong>" + name + ":</strong> " + (value | @html) + "</li>\n"
	end;
"<p>" + (.description | @html) + "</p>\n<ul>\n"
+ line("Role"; .role + " (" + .department + ")")
+ line("Director"; .director | join(", "))
+ line("Production"; .prod_co | join(", "))
+ line("Country"; .prod_country | join(", "))
+ line("Platforms"; .platforms | join(", "))
+ line("Released"; if .release_year > 0 then (.release_year | tostring) else .release_status end)
+ "</ul>\n"
+ if .imdb_link == "" then "" else "<p><a href=\"" + (.imdb_link | @html) + "\">IMDb page</a></p>" end
'

count=$(jq 'length' "$work/films.json")
created=0

# (( ... )): arithmetic; the loop runs i = 0, 1, ... count - 1
for (( i = 0; i < count; i++ ))
do
	film=$(jq -c --argjson i "$i" '.[$i]' "$work/films.json")
	id=$(echo "$film" | jq -r '.id')
	title=$(echo "$film" | jq -r '.title')
	type=$(echo "$film" | jq -r '.type')
	date=$(echo "$film" | jq -r '.job_start')
	image="$work/images/$(echo "$film" | jq -r '.image_id').jpg"
	# tags: department, role and countries, separated by commas
	tags=$(echo "$film" | jq -r '[.department, .role] + .prod_country | join(",")')
	content=$(echo "$film" | jq -r "$page")

	# STEP 3: POSTS
	# every imported post stores the film id (film_id); if one already
	# has this id, the film was imported before: skip it
	found=$(runuser -u www-data -- wp --path="$site" post list --post_type=post --post_status=any --meta_key=film_id --meta_value="$id" --format=count)
	if [ "$found" -gt 0 ]
	then
		echo "skip: $title (already imported)"
		continue
	fi

	# --post_date: the shoot start date, so posts sort by when you worked
	# --meta_input: extra data saved with the post, as JSON
	# --post_author=1: the admin account made during the WordPress setup
	# --porcelain: print only the new post's id
	post_id=$(runuser -u www-data -- wp --path="$site" post create \
		--post_title="$title" \
		--post_content="$content" \
		--post_status=publish \
		--post_author=1 \
		--post_date="$date 12:00:00" \
		--post_category="$type" \
		--tags_input="$tags" \
		--meta_input="{\"film_id\":\"$id\"}" \
		--porcelain)
	echo "post $post_id: $title"
	created=$((created + 1))

	# STEP 4: IMAGES
	# media import uploads the picture to the media library;
	# --featured_image makes it the post's main picture;
	# the picture also gets film_id so the undo command finds it
	if [ -f "$image" ]
	then
		image_id=$(runuser -u www-data -- wp --path="$site" media import "$image" --post_id="$post_id" --title="$title" --featured_image --porcelain)
		runuser -u www-data -- wp --path="$site" post meta add "$image_id" film_id "$id" > /dev/null
	fi
done

echo "done: $created new posts"
