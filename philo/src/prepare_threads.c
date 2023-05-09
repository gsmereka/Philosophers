/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_threads.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsmereka <gsmereka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 18:04:11 by gsmereka          #+#    #+#             */
/*   Updated: 2023/05/09 13:15:52 by gsmereka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philo.h"

int	get_start_time(t_data *data)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	data->start_time = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (0);
}

long int get_time_data()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000) + (time.tv_usec / 1000);
}

void *observer_routine(void *observer_data)
{
    t_data		*data;
	long int 	last_meal_time;
	long int	current_time;
	int			i;
    int 		stop;

	data = (t_data *)observer_data;
	stop = 0;
    while (!stop)
	{
		i = 0;
		while (i < data->config->number_of_philosophers)
		{
            pthread_mutex_lock(data->philosophers[i]->shared->last_meal_mutex);
            last_meal_time = data->philosophers[i]->shared->last_meal_time;
            pthread_mutex_unlock(data->philosophers[i]->shared->last_meal_mutex);
            current_time = get_time_data();
			current_time -= data->start_time;
            if (current_time - last_meal_time > data->config->time_to_die)
			{
                printf("%ld %d died\n", current_time, data->philosophers[i]->id);
				pthread_mutex_lock(data->need_stop_mutex);
                data->need_stop = TRUE;
				pthread_mutex_lock(data->need_stop_mutex);
                stop = 1;
                break;
            }
			i++;
        }
    }
}

void	prepare_threads(t_data *data)
{
	int	philo;

	philo = 0;
	pthread_mutex_init(data->need_stop_mutex, NULL);
	pthread_mutex_init(data->start_time_mutex, NULL);
	while ((philo < data->config->number_of_philosophers))
	{
		pthread_mutex_init(data->forks[philo]->mutex, NULL);
		philo++;
	}
	philo = 0;
	get_start_time(data);
	pthread_create(data->observer_thread, NULL, &observer_routine, data);
	while ((philo < data->config->number_of_philosophers))
	{
		pthread_mutex_init(data->philosophers[philo]->shared->last_meal_mutex, NULL);
		pthread_create(data->philo_threads[philo], NULL, &philosopher_routine, data->philosophers[philo]);
		philo++;
	}
	philo = 0;
	pthread_join((*data->observer_thread), NULL);
	while ((philo < data->config->number_of_philosophers))
	{
		pthread_join((*data->philo_threads[philo]), NULL);
		philo++;
	}
}
